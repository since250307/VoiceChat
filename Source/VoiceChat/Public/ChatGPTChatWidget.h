#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatGPTChatWidget.generated.h"

UCLASS()
class VOICECHAT_API UChatGPTChatWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 블루프린트에서 Chat 기록을 갱신할 수 있도록 이벤트 선언
    UFUNCTION(BlueprintImplementableEvent, Category = "Chat")
    void UpdateChat(const FString& GPTMessage);

    // 사용자가 입력한 메시지를 전달하기 위한 BlueprintCallable 함수
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void SendMessageToGPT(const FString& UserMessage);
};
