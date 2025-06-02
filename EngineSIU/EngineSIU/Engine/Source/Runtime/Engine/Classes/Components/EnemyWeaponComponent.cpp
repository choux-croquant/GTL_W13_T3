#include "EnemyWeaponComponent.h"
#include "Actors/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Contents/AnimInstance/LuaScriptAnimInstance.h"
#include "Animation/AnimStateMachine.h"

void UEnemyWeaponComponent::GotParried(float InDamage)
{
    UE_LOG(ELogLevel::Error,"GotParried");
    AEnemy* EnemyActor = Cast<AEnemy>(GetOwner());

    ULuaScriptAnimInstance* AnimInstance = Cast<ULuaScriptAnimInstance>(EnemyActor->SkeletalMeshComponent->GetAnimInstance());

    if (UAnimStateMachine* StateMachine = AnimInstance->GetStateMachine())
    {
        StateMachine->State = FString("Reacting");
    }
}
