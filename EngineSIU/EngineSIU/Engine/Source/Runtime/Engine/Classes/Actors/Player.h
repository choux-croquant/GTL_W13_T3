#pragma once
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectTypes.h"
#include "Particles/ParticleSystem.h"

class USocketComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UGizmoBaseComponent;
class UGizmoArrowComponent;
class USceneComponent;
class UPrimitiveComponent;
class FEditorViewportClient;
class UStaticMeshComponent;

class AEditorPlayer : public AActor
{
    DECLARE_CLASS(AEditorPlayer, AActor)

    AEditorPlayer() = default;

    virtual void Tick(float DeltaTime) override;

    void Input();
    bool PickGizmo(FVector& RayOrigin, FEditorViewportClient* InActiveViewport);
    void ProcessGizmoIntersection(UStaticMeshComponent* Component, const FVector& PickPosition, FEditorViewportClient* InActiveViewport, bool& bIsPickedGizmo);
    void PickActor(const FVector& PickPosition);
    void AddControlMode();
    void AddCoordMode();
    void SetCoordMode(ECoordMode InMode) { CoordMode = InMode; }

private:
    static int RayIntersectsObject(const FVector& PickPosition, USceneComponent* Component, float& HitDistance, int& IntersectCount);
    void ScreenToViewSpace(int32 ScreenX, int32 ScreenY, std::shared_ptr<FEditorViewportClient> ActiveViewport, FVector& RayOrigin);
    void PickedObjControl();
    void PickedBoneControl();
    
    void ControlRotation(USceneComponent* Component, UGizmoBaseComponent* Gizmo, float DeltaX, float DeltaY);
    
    void ControlScale(USceneComponent* Component, UGizmoBaseComponent* Gizmo, float DeltaX, float DeltaY);
    FQuat ControlBoneRotation(FTransform& Component, UGizmoBaseComponent* Gizmo, float DeltaX, float DeltaY);
    FVector ControlBoneScale(FTransform& Component, UGizmoBaseComponent* Gizmo, float DeltaX, float DeltaY);
    

    bool bLeftMouseDown = false;

    POINT LastMousePos;
    EControlMode ControlMode = CM_TRANSLATION;
    ECoordMode CoordMode = CDM_WORLD;
    FQuat InitialBoneRotationForGizmo;

public:
    void SetMode(EControlMode Mode) { ControlMode = Mode; }
    EControlMode GetControlMode() const { return ControlMode; }
    ECoordMode GetCoordMode() const { return CoordMode; }
};

class APlayer : public AActor
{
    DECLARE_CLASS(APlayer, AActor)

public:
    APlayer() = default;

    virtual UObject* Duplicate(UObject* InOuter) override;
    virtual void Tick(float DeltaTime) override;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32 /* CurrentHealth */, int32 /* MaxHealth */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHeroDied, bool /* DieByHealth */);
DECLARE_MULTICAST_DELEGATE(FOnParry);
DECLARE_MULTICAST_DELEGATE(FOnCinematicFinish);
// DECLARE_MULTICAST_DELEGATE_

class AHeroPlayer : public APlayer
{
    DECLARE_CLASS(AHeroPlayer, APlayer)
public:
    
    AHeroPlayer() = default;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void GetDamaged(float Damage);
    void Parry(AActor* OverlappedActor, AActor* OtherActor);
    virtual UObject* Duplicate(UObject* InOuter) override;
    virtual void Tick(float DeltaTime) override;
    virtual void PostSpawnInitialize() override;

    void SetAnimState(FString InState);
    FName GetStateMachine();
    void ResetHero();
    void OnFinalScene();
    void OnHeroDie();

    FOnHealthChanged OnHealthChanged;
    FOnHeroDied OnHeroDied;
    FOnParry OnParry;
    FOnCinematicFinish OnCinematicFinish;

    void SetHealth(float InHealth);
    bool IsDead();
    float GetHealth();
    float GetMaxHealth();

    bool bWaitingStart = true;

    bool bIsParrying = false;

    USocketComponent* CameraSocketComponent = nullptr;
    
    
private:
    // UPROPERTY
    // (EditAnywhere, USkeletalMeshComponent*, OriginSkeletalMeshComponent, = nullptr)
    UPROPERTY
    (EditAnywhere, float, MaxHealth, = 100.f);
    UPROPERTY
    (EditAnywhere, float, Health, = 3.f);
    int32 CameraMoveCounter = 0;

    FTransform InitialActorTransform;

    UParticleSystem* SparkParticle = nullptr;
    UParticleSystem* FogParticle = nullptr;
};

