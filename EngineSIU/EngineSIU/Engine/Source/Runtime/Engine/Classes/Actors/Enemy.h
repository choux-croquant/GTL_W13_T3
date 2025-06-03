#pragma once
#include "GameFramework/Actor.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;
class UAnimCustomNotify;
class UAnimSoundNotify;
class UAnimSequence;

enum EAttackDirection
{
    AD_None,
    AD_Vertical,
    AD_Horizontal,
};

class AEnemy : public AActor
{
    DECLARE_CLASS(AEnemy, AActor)
public:
    AEnemy() = default;
    
    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    
    UObject* Duplicate(UObject* InOuter);

    void HandleAttackNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, EAttackDirection InAttackDirection);

    void HandleAttackNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

    void CreateAttackNotify(UAnimSequence* AnimSequence, UAnimCustomNotify*& OutNotify, const FString& NotifyName, float TriggerTime);

    void CreateSoundNotify(UAnimSequence* AnimSequence, UAnimSoundNotify*& OutNotify, const FString& NotifyName, const FString& SoundName, float TriggerTime);

    void BindAttackNotifies();

    void ResetEnemyProperties();
public:
    FTransform InitialTransform;

    UAnimCustomNotify* AttackToIdleNotify = nullptr;
    UAnimCustomNotify* AttackVerticalNotify = nullptr;
    UAnimCustomNotify* AttackHorizontalNotify = nullptr;
    UAnimCustomNotify* AttackVerticalNotifyEnd = nullptr;
    UAnimCustomNotify* AttackHorizontalNotifyEnd = nullptr;

    UAnimSoundNotify* ReactionNotify = nullptr;
    UAnimSoundNotify* PlayerHitNotify = nullptr;

    EAttackDirection CurrentAttackDirection = AD_None;

    UPROPERTY(EditAnywhere | EditInline, USkeletalMeshComponent*, SkeletalMeshComponent, = nullptr)

    float ParryGauge = 0.0f;
};
