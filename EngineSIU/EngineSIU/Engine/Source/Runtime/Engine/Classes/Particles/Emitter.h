#pragma once
#include "GameFramework/Actor.h"
#include "ParticleSystemComponent.h"

class AEmitter : public AActor
{
	DECLARE_CLASS(AEmitter, AActor)

    AEmitter() = default;
    virtual ~AEmitter() override = default;

    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;
    virtual UObject* Duplicate(UObject* InOuter) override;
    
    UPROPERTY(EditAnywhere | EditInline, UParticleSystemComponent*, ParticleSystemComponent, = nullptr)
    
};
