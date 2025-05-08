#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChatGPTTestActor.generated.h"

UCLASS()
class VOICECHAT_API AChatGPTTestActor : public AActor
{
    GENERATED_BODY()

public:
    AChatGPTTestActor();

protected:
    virtual void BeginPlay() override;
};
