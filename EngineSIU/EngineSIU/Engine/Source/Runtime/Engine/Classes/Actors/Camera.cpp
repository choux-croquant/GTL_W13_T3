#include "Camera.h"

#include "Camera/PlayerCameraManager.h"
#include "Engine/Engine.h"
#include "World/World.h"

void ACamera::PostSpawnInitialize()
{
    AActor::PostSpawnInitialize();

    CameraComponent = AddComponent<UCameraComponent>("CameraComponent");
}

void ACamera::BeginPlay()
{
    AActor::BeginPlay();
}

void ACamera::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);
}

UObject* ACamera::Duplicate(UObject* InOuter)
{
    ACamera* NewActor = Cast<ACamera>(Super::Duplicate(InOuter));
    NewActor->CameraComponent = NewActor->GetComponentByClass<UCameraComponent>();
    
    return NewActor;
}
