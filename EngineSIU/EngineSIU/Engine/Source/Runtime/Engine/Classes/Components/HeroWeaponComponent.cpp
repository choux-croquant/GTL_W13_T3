#include "HeroWeaponComponent.h"

#include "EnemyWeaponComponent.h"
#include "UObject/Casts.h"

UHeroWeaponComponent::UHeroWeaponComponent()
{
     
}

void UHeroWeaponComponent::BeginPlay()
{
    UBoxComponent::BeginPlay();

    SetParryMode(true); //이거 animnotify로 켰다껐다
    
    OnComponentBeginOverlap.AddLambda(
        [this](UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit){
            Parry(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);
        }
    );
}

void UHeroWeaponComponent::SetParryMode(bool InbIsParry)
{
    bIsParrying = InbIsParry;
}

void UHeroWeaponComponent::Parry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (bIsParrying == false)
    {
        return;
    }
    
    // if (OtherComp->IsA<UEnemyWeaponComponent>())
    if (UEnemyWeaponComponent* EnemyWeaponComponent = Cast<UEnemyWeaponComponent>(OtherComp))
    {
        EnemyWeaponComponent->GotParried();
    }
    
}
