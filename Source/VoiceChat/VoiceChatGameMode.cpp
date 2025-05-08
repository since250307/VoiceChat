// VoiceChatGameMode.cpp

#include "VoiceChatGameMode.h"
#include "VoiceChatCharacter.h"
#include "VoiceChatHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OpenAIRequester.h"
#include "ChatGPTChatWidget.h"

AVoiceChatGameMode::AVoiceChatGameMode()
{
    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode Constructor: START")); // <--- 추가

    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
    if (PlayerPawnClassFinder.Succeeded())
    {
        DefaultPawnClass = PlayerPawnClassFinder.Class;
        UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode Constructor: DefaultPawnClass FOUND."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AVoiceChatGameMode Constructor: FAILED to find DefaultPawnClass. Check path."));
    }

    HUDClass = AVoiceChatHUD::StaticClass();

    // ✅ 위젯 클래스 경로는 무조건 생성자에서!
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBPClass(TEXT("/Game/BluePrint/WBP_ChatGPTChat")); // 사용자가 확인한 정확한 경로
    if (WidgetBPClass.Succeeded())
    {
        WidgetClass = WidgetBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode Constructor: Successfully found WBP_ChatGPTChat class!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AVoiceChatGameMode Constructor: FAILED to find WBP_ChatGPTChat class at path /Game/BluePrint/WBP_ChatGPTChat. Please double-check the path."));
        WidgetClass = nullptr;
    }
    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode Constructor: END")); // <--- 추가
}

void AVoiceChatGameMode::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() EXECUTED - START")); // <--- 이 로그를 추가합니다.
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() - After Super::BeginPlay()")); // <--- 이것도 추가
    // ✅ 1. Requester 생성 및 바인딩
    Requester = NewObject<UOpenAIRequester>(this);
    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() - Requester Object Created")); // <--- 이것도 추가
    if (Requester)
    {
        UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() - Requester is VALID, binding delegate...")); // <--- 이것도 추가
        Requester->OnGPTResponseReceived.AddDynamic(this, &AVoiceChatGameMode::OnGPTResponse);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AVoiceChatGameMode::BeginPlay() - Requester is NULL!")); // <--- 이것도 추가
    }

    // ✅ 2. 위젯 생성
    if (WidgetClass)
    {
        ChatWidget = CreateWidget<UChatGPTChatWidget>(GetWorld(), WidgetClass);
        if (ChatWidget)
        {
            ChatWidget->AddToViewport();
        }
    }

    // ✅ 3. API 요청
    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() - Attempting to send chat request..."));
    if (Requester)
    {
        Requester->SendChatRequest(TEXT("안녕 GPT! Unreal에서 보내는 메시지야."));
        UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() - SendChatRequest CALLED on Requester."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AVoiceChatGameMode::BeginPlay() - Cannot SendChatRequest, Requester is NULL."));
    }
    UE_LOG(LogTemp, Warning, TEXT("AVoiceChatGameMode::BeginPlay() EXECUTED - END")); // <--- 이것도 추가
}

void AVoiceChatGameMode::OnGPTResponse(const FString& ResponseText)
{
    UE_LOG(LogTemp, Warning, TEXT("📥 GPT 응답 도착: %s"), *ResponseText);

    if (ChatWidget)
    {
        ChatWidget->UpdateChat(ResponseText);
    }
}
