#pragma once
#include "BoxComponent.h"
#include "PrimitiveComponent.h"

class UEnemyWeaponComponent : public UBoxComponent
{
    DECLARE_CLASS(UEnemyWeaponComponent, UBoxComponent)
public:
    UEnemyWeaponComponent() = default;

    void GotParried();
};
