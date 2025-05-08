#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "ChatGPTClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatGPTResponseReceived, const FString&, Response);

UCLASS()
class VOICECHAT_API UChatGPTClient : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SendMessageToChatGPT(const FString& Message);

    UPROPERTY(BlueprintAssignable)
    FOnChatGPTResponseReceived OnResponseReceivedEvent;

private:
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
