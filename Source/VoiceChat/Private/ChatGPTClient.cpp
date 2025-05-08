#include "ChatGPTClient.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

void UChatGPTClient::SendMessageToChatGPT(const FString& Message)
{
    FString APIKey = TEXT("sk-..."); // 🔥 여기에 진짜 API 키 입력
    FString APIUrl = TEXT("https://api.openai.com/v1/chat/completions");

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UChatGPTClient::OnResponseReceived);
    Request->SetURL(APIUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization"), FString("Bearer ") + APIKey);

    // JSON 본문 만들기
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField("model", "gpt-3.5-turbo");

    TArray<TSharedPtr<FJsonValue>> Messages;
    TSharedPtr<FJsonObject> MsgObject = MakeShareable(new FJsonObject);
    MsgObject->SetStringField("role", "user");
    MsgObject->SetStringField("content", Message);
    Messages.Add(MakeShareable(new FJsonValueObject(MsgObject)));

    JsonObject->SetArrayField("messages", Messages);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(OutputString);
    Request->ProcessRequest();
}

void UChatGPTClient::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid()) return;

    FString ResponseString = Response->GetContentAsString();

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* Choices;
        if (JsonObject->TryGetArrayField("choices", Choices) && Choices->Num() > 0)
        {
            FString Content = (*Choices)[0]->AsObject()->GetObjectField("message")->GetStringField("content");
            OnResponseReceivedEvent.Broadcast(Content);
        }
    }
}
