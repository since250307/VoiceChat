// ChatGPTTestActor.cpp

#include "ChatGPTTestActor.h"
#include "ChatGPTClient.h"

AChatGPTTestActor::AChatGPTTestActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AChatGPTTestActor::BeginPlay()
{
    Super::BeginPlay();

    UChatGPTClient* ChatClient = NewObject<UChatGPTClient>();
    if (ChatClient)
    {
        ChatClient->SendMessageToChatGPT(TEXT("안녕 ChatGPT! 오늘 기분 어때?"));
    }
}
