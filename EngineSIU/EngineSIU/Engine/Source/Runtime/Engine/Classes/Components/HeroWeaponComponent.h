#pragma once
#include "BoxComponent.h"

class UHeroWeaponComponent : public UBoxComponent
{
    DECLARE_CLASS(UHeroWeaponComponent, UBoxComponent)
public:
    UHeroWeaponComponent();
    
    virtual void BeginPlay() override;
    void SetParryMode(bool InbIsParry);
    void Parry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

    UPROPERTY
    (EditAnywhere, bool, bIsParrying, = 0)
};
