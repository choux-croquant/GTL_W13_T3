#include "EnemyWeaponComponent.h"
#include "Actors/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Contents/AnimInstance/LuaScriptAnimInstance.h"
#include "Animation/AnimStateMachine.h"
#include "SoundManager.h"

void UEnemyWeaponComponent::GotParried(float InDamage)
{
    UE_LOG(ELogLevel::Error,"GotParried");
    AEnemy* EnemyActor = Cast<AEnemy>(GetOwner());
    // 데미지 조정 필요 시 Player속성으로 가지도록 이동
    EnemyActor->ParryGauge += 35.0f;

    ULuaScriptAnimInstance* AnimInstance = Cast<ULuaScriptAnimInstance>(EnemyActor->SkeletalMeshComponent->GetAnimInstance());

    if (EnemyActor->ParryGauge > 100.0f)
    {
        if (UAnimStateMachine* StateMachine = AnimInstance->GetStateMachine())
        {
            FSoundManager::GetInstance().PlaySound("Parry");
            StateMachine->ChangeStateMachineLua(FString("Defeat"));
            //EnemyActor->SkeletalMeshComponent->ChangeRigidBodyFlag(ERigidBodyType::DYNAMIC);
            //EnemyActor->SkeletalMeshComponent->bSimulate = true;
        }
    }
    else 
    {
        if (UAnimStateMachine* StateMachine = AnimInstance->GetStateMachine())
        {
            StateMachine->ChangeStateMachineLua(FString("Reacting"));
            //EnemyActor->SkeletalMeshComponent->ChangeRigidBodyFlag(ERigidBodyType::DYNAMIC);
            //EnemyActor->SkeletalMeshComponent->bSimulate = true;
        }
    }
}
