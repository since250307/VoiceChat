#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OpenAIRequester.h"
#include "ChatGPTChatWidget.h"
#include "VoiceChatGameMode.generated.h"

UCLASS(minimalapi)
class AVoiceChatGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AVoiceChatGameMode();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UOpenAIRequester* Requester;

    UPROPERTY()
    UChatGPTChatWidget* ChatWidget;

    UPROPERTY()
    TSubclassOf<UUserWidget> WidgetClass;

    UFUNCTION()
    void OnGPTResponse(const FString& ResponseText);
};
