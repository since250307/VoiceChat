// VoiceChatHUD.cpp

#include "VoiceChatHUD.h"
#include "Blueprint/UserWidget.h"

void AVoiceChatHUD::BeginPlay()
{
    Super::BeginPlay();

    if (UClass* ChatWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/WBP_ChatGPTChat.WBP_ChatGPTChat_C")))
    {
        UUserWidget* ChatWidget = CreateWidget<UUserWidget>(GetWorld(), ChatWidgetClass);
        if (ChatWidget)
        {
            ChatWidget->AddToViewport();
        }
    }
}
