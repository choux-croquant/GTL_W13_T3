#pragma once
#include "GameFramework/Actor.h"

class USkeletalMeshComponent;

class AEnemy : public AActor
{
    DECLARE_CLASS(AEnemy, AActor)
public:
    AEnemy() = default;
    
    void PostSpawnInitialize();

    void BeginPlay();

    UObject* Duplicate(UObject* InOuter);

    UPROPERTY(EditAnywhere | EditInline, USkeletalMeshComponent*, SkeletalMeshComponent, = nullptr)
};
