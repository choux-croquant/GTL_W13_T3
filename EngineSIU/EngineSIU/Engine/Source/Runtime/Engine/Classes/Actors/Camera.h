#pragma once
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

class ACamera : public AActor
{
    DECLARE_CLASS(ACamera, AActor)
public:
    ACamera() = default;
    
    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual UObject* Duplicate(UObject* InOuter) override;

    UPROPERTY(EditAnywhere | EditInline, UCameraComponent*, CameraComponent, = nullptr)
};
