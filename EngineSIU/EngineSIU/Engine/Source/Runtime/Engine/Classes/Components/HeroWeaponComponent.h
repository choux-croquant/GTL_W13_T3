#pragma once
#include "BoxComponent.h"
#include "Actors/Enemy.h"

class UHeroWeaponComponent : public UBoxComponent
{
    DECLARE_CLASS(UHeroWeaponComponent, UBoxComponent)
public:
    UHeroWeaponComponent();
    
    virtual void BeginPlay() override;
    float GetStateDamage(FName InState);
    EAttackDirection GetParryDirection(FName InState);
    void OnWeaponOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

    // UPROPERTY
    // (EditAnywhere, bool, bIsParrying, = 0)
};
