#include "Emitter.h"

#include "Animation/SkeletalMeshActor.h"

void AEmitter::PostSpawnInitialize()
{
    AActor::PostSpawnInitialize();

    ParticleSystemComponent = AddComponent<UParticleSystemComponent>("ParticleSystemComponent");
}

void AEmitter::BeginPlay()
{
    AActor::BeginPlay();
}

UObject* AEmitter::Duplicate(UObject* InOuter)
{
    AEmitter* NewActor = Cast<AEmitter>(Super::Duplicate(InOuter));

    NewActor->ParticleSystemComponent = NewActor->GetComponentByClass<UParticleSystemComponent>();

    return NewActor;
}

