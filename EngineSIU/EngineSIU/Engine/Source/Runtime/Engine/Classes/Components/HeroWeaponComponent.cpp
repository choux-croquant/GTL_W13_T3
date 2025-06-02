#include "HeroWeaponComponent.h"

#include "EnemyWeaponComponent.h"
#include "Actors/Enemy.h"
#include "Actors/Player.h"
#include "UObject/Casts.h"

UHeroWeaponComponent::UHeroWeaponComponent()
{
     
}

void UHeroWeaponComponent::BeginPlay()
{
    UBoxComponent::BeginPlay();
    
    OnComponentBeginOverlap.AddLambda(
        [this](UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit){
            OnWeaponOverlapped(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);
        }
    );
}

float UHeroWeaponComponent::GetStateDamage(FName InState)
{
    if (InState == TEXT("VerticalFastParry") || InState == TEXT("HorizontalFastParry"))
    {
        //변수로 관리
        return 5.f;
    }

    if (InState == TEXT("VerticalHardParry") || InState == TEXT("HorizontalHardParry"))
    {
        //변수로 관리
        return 8.f;
    }

    return 0.f;
}

EAttackDirection UHeroWeaponComponent::GetParryDirection(FName InState)
{
    //세로로 내리치니까 가로로 오는걸 막음
    if (InState == TEXT("VerticalFastParry") || InState == TEXT("VerticalHardParry"))
    {
        return AD_Horizontal;
    }

    if (InState == TEXT("HorizontalFastParry") || InState == TEXT("HorizontalHardParry"))
    {
        return AD_Vertical;
    }

    return AD_None;
}

void UHeroWeaponComponent::OnWeaponOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
    {
        if (UEnemyWeaponComponent* EnemyWeaponComponent = Cast<UEnemyWeaponComponent>(OtherComp))
        {
            AHeroPlayer* Player = Cast<AHeroPlayer>(GetOwner());
             
            if (Enemy->CurrentAttackDirection != AD_None)
            {
                Player->Parry(Player, Enemy);
                EnemyWeaponComponent->GotParried(GetStateDamage(Player->GetStateMachine()));
                
            }
        }
    }
}
