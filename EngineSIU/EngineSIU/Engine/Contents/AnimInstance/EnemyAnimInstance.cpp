#include "EnemyAnimInstance.h"
#include "Engine/AssetManager.h"

UEnemyAnimInstance::UEnemyAnimInstance()
    : PlayRate(1.f)
    , ElapsedTime(0.f)
    , bLooping(true)
    , bPlaying(true)
    , bReverse(false)
{
    CurrentState = EEnemyAnimState::Idle;

    IdleAnim = UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Idle/Enemy_IdleArmature|Armature|mixamo.com|Layer0"));
    ReactionAnim = UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Impact/Enemy_ImpactArmature|Enemy_Impact"));
    AttackAnims.Add(UAssetManager::Get().GetAnimation(FString("Contents/Combo_RLU/Combo_RLUArmature|Combo_RLU")));
    AttackAnims.Add(UAssetManager::Get().GetAnimation(FString("Contents/GameJamEnemy/GameJamEnemyArmature|Combo_RLR")));
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
    Super::NativeUpdateAnimation(DeltaSeconds, OutPose);

    // 상태 머신 업데이트 로직 (필요 시 구현)
}

void UEnemyAnimInstance::StartAttack()
{
    if (CurrentState != EEnemyAnimState::Idle || !bCanAttack)
        return;

    TransitionToState(EEnemyAnimState::Attacking);

    // 랜덤 공격 애니메이션 선택
    UAnimSequence* SelectedAttack = GetRandomAttackAnim();
    //if (SelectedAttack)
    //{
    //    // 애니메이션 재생 로직 (Montage 사용 권장)
    //    // Montage_Play(SelectedAttack);

    //    // 공격 후 자동으로 Idle로 복귀
    //    const float AttackDuration = SelectedAttack->GetPlayLength();
    //    GetWorld()->GetTimerManager().SetTimer(
    //        AttackTimerHandle,
    //        this,
    //        &UEnemyAnimInstance::TransitionToIdle,
    //        AttackDuration,
    //        false
    //    );
    //}
}

void UEnemyAnimInstance::TriggerReaction()
{
    if (CurrentState == EEnemyAnimState::Reacting)
        return;

    TransitionToState(EEnemyAnimState::Reacting);

    // 반응 애니메이션 재생
    //if (ReactionAnim)
    //{
    //    // Montage_Play(ReactionAnim);

    //    const float ReactionDuration = ReactionAnim->GetPlayLength();
    //    GetWorld()->GetTimerManager().SetTimer(
    //        AttackTimerHandle,
    //        this,
    //        &UEnemyAnimInstance::TransitionToIdle,
    //        ReactionDuration,
    //        false
    //    );
    //}
}

UAnimSequence* UEnemyAnimInstance::GetRandomAttackAnim() const
{
    if (AttackAnims.Num() == 0)
        return nullptr;

    //const int32 RandomIndex = FMath::RandRange(0, AttackAnims.Num() - 1);
    //return AttackAnims[RandomIndex];
    return nullptr;
}

void UEnemyAnimInstance::ResetAttackTimer()
{
   /* const float NextAttackTime = FMath::FRandRange(
        AttackIntervalRange.X,
        AttackIntervalRange.Y
    );

    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &UEnemyAnimInstance::StartAttack,
        NextAttackTime,
        false
    );*/
}

//void UEnemyAnimInstance::TransitionToIdle()
//{
    //TransitionToState(EEnemyAnimState::Idle);
    //ResetAttackTimer();
//}

void UEnemyAnimInstance::TransitionToState(EEnemyAnimState NewState)
{
    CurrentState = NewState;
    bCanAttack = (NewState == EEnemyAnimState::Idle);
}
