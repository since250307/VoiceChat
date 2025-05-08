#include "OpenAIRequester.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/ConfigCacheIni.h"

void UOpenAIRequester::SendChatRequest(const FString& UserMessage)
{
    FString APIKey; // 빈 문자열로 초기화
    // DefaultGame.ini 파일에서 API 키 읽어오기
    if (!GConfig->GetString(
        TEXT("OpenAISettings"), // .ini 파일에 정의한 섹션 이름
        TEXT("APIKey"),         // .ini 파일에 정의한 키 이름
        APIKey,                 // 읽어온 값을 저장할 변수
        GGameIni                // 대상 .ini 파일 (GGameUserSettingsIni도 사용 가능)
    ))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read APIKey from DefaultGame.ini. Please check OpenAISettings section."));
        // API 키를 읽지 못한 경우, 오류 처리 또는 기본 동작을 정의할 수 있습니다.
        // 예를 들어, 요청을 보내지 않거나, 사용자에게 알림을 줄 수 있습니다.
        return; // API 키가 없으면 요청을 보내지 않음
    }

    if (APIKey.IsEmpty() || APIKey.Equals(TEXT("여기에_새로_생성한_API_키를_입력하세요"))) // .ini 파일에 키가 없거나 기본값일 경우
    {
        UE_LOG(LogTemp, Error, TEXT("APIKey is not set in DefaultGame.ini or still has the default placeholder."));
        return; // API 키가 설정되지 않았으면 요청을 보내지 않음
    }


    FString Url = TEXT("https://api.openai.com/v1/chat/completions");

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Authorization", "Bearer " + APIKey); // <--- 읽어온 APIKey 사용

    // ... (이하 JSON Payload 생성 및 요청 보내는 코드는 동일) ...
    // JSON Payload 생성
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    RootObject->SetStringField("model", "gpt-3.5-turbo");

    TArray<TSharedPtr<FJsonValue>> Messages;
    TSharedPtr<FJsonObject> UserMsg = MakeShareable(new FJsonObject);
    UserMsg->SetStringField("role", "user");
    UserMsg->SetStringField("content", UserMessage);
    Messages.Add(MakeShareable(new FJsonValueObject(UserMsg)));

    RootObject->SetArrayField("messages", Messages);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);
    Request->OnProcessRequestComplete().BindUObject(this, &UOpenAIRequester::OnResponseReceived);
    Request->ProcessRequest();
}

void UOpenAIRequester::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ OnResponseReceived 호출됨"));

        if (!bWasSuccessful || !Response.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("❌ OpenAI 요청 실패"));
            return;
        }

        FString ResponseStr = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("📨 GPT 응답 원문: %s"), *ResponseStr);
        // JSON 파싱해서 message.content 추출
        TSharedPtr<FJsonObject> JsonObj;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
        if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>* Choices;
            if (JsonObj->TryGetArrayField("choices", Choices))
            {
                if (Choices->Num() > 0)
                {
                    TSharedPtr<FJsonObject> MessageObj = (*Choices)[0]->AsObject()->GetObjectField("message");
                    FString Content = MessageObj->GetStringField("content");

                    // 🔥 Delegate 호출
                    OnGPTResponseReceived.Broadcast(Content);
                    return;
                }
            }
        }
        UE_LOG(LogTemp, Error, TEXT("GPT 응답 파싱 실패"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OpenAI 요청 실패"));
    }
}
