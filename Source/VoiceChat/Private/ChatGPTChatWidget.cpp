#include "ChatGPTChatWidget.h"


void UChatGPTChatWidget::SendMessageToGPT(const FString& UserMessage)
{
    // 예시: 이 함수는 블루프린트에서 Override할 것이므로, C++에서는 비워도 됨
    UE_LOG(LogTemp, Warning, TEXT("SendMessageToGPT called with: %s"), *UserMessage);

}