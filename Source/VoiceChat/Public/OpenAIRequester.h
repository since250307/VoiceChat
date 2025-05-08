#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "OpenAIRequester.generated.h"

// 델리게이트 선언: FString 하나를 파라미터로 받는 형식
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGPTResponseReceived, const FString&, ResponseText);

UCLASS(BlueprintType)
class VOICECHAT_API UOpenAIRequester : public UObject
{
    GENERATED_BODY()

public:
    // API 호출 함수: 블루프린트에서 호출 가능
    UFUNCTION(BlueprintCallable, Category = "OpenAI")
    void SendChatRequest(const FString& UserMessage);

    // 블루프린트에서 바인딩 가능한 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "OpenAI")
    FOnGPTResponseReceived OnGPTResponseReceived;

private:
    // HTTP 응답 콜백 처리
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
