#pragma once
#include "GameFramework/Actor.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;
class UAnimCustomNotify;

class AEnemy : public AActor
{
    DECLARE_CLASS(AEnemy, AActor)
public:
    AEnemy() = default;
    
    void PostSpawnInitialize();

    void BeginPlay();

    UObject* Duplicate(UObject* InOuter);

    void HandleAttackNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
    UAnimCustomNotify* EnemyAttackNotify;

    UPROPERTY(EditAnywhere | EditInline, USkeletalMeshComponent*, SkeletalMeshComponent, = nullptr)
};
