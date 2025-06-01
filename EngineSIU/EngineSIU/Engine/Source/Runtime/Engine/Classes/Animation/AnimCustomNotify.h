#pragma once

#include "Animation/AnimNotify.h"

class USkeletalMeshComponent;

class UAnimCustomNotify : public UAnimNotify
{
    DECLARE_CLASS(UAnimCustomNotify, UAnimNotify)

public:
    UAnimCustomNotify();
    ~UAnimCustomNotify() = default;

    DECLARE_MULTICAST_DELEGATE_TwoParams(FCustomNotifyDelegate, USkeletalMeshComponent* MeshComp, UAnimSequenceBase*, Animation);

    FCustomNotifyDelegate OnCustomNotify;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
