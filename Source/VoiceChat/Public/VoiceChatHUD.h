// VoiceChatHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VoiceChatHUD.generated.h"

UCLASS()
class VOICECHAT_API AVoiceChatHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
