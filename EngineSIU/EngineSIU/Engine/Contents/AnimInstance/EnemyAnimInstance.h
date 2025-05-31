#pragma once

#include "Animation/AnimInstance.h"

enum class EEnemyAnimState : uint8
{
    Idle,       
    Attacking,  
    Reacting 
};

class UEnemyAnimInstance : public UAnimInstance
{
    DECLARE_CLASS(UEnemyAnimInstance, UAnimInstance)
public:
    UEnemyAnimInstance();
    ~UEnemyAnimInstance() = default;

    UAnimationAsset* IdleAnim;

    TArray<UAnimationAsset*> AttackAnims;

    UAnimationAsset* ReactionAnim;

    EEnemyAnimState CurrentState;

    // 공격 간격
    FVector2D AttackIntervalRange = FVector2D(3.0f, 5.0f);
private:
    float ElapsedTime;

    float PlayRate;

    bool bCanAttack = true;

    bool bLooping;

    bool bPlaying;

    bool bReverse;
public:
    virtual void NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose) override;

    void StartAttack();

    void TriggerReaction();

    void SetPlaying(bool bIsPlaying)
    {
        bPlaying = bIsPlaying;
    }

    bool IsPlaying() const
    {
        return bPlaying;
    }

    void SetReverse(bool bIsReverse)
    {
        bReverse = bIsReverse;
    }

    bool IsReverse() const
    {
        return bReverse;
    }

    void SetLooping(bool bIsLooping)
    {
        bLooping = bIsLooping;
    }

    bool IsLooping() const
    {
        return bLooping;
    }
protected:
    // 랜덤 공격 선택
    UAnimSequence* GetRandomAttackAnim() const;

    // 공격 타이머 초기화
    void ResetAttackTimer();

    // 상태 전환
    void TransitionToState(EEnemyAnimState NewState);
};
