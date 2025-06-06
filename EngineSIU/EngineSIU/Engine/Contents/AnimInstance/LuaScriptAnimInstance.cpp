#include "LuaScriptAnimInstance.h"

#include "Animation/AnimationAsset.h"
#include "Animation/AnimationRuntime.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Misc/FrameTime.h"
#include "Animation/AnimStateMachine.h"
#include "UObject/Casts.h"
#include "UObject/ObjectFactory.h"
#include "GameFramework/Pawn.h"

ULuaScriptAnimInstance::ULuaScriptAnimInstance()
    : PrevAnim(nullptr)
    , CurrAnim(nullptr)
    , PreviousTime(0.f)
    , ElapsedTime(0.f)
    , PlayRate(1.f)
    , bLooping(true)
    , bPlaying(true)
    , bReverse(false)
    , LoopStartFrame(0)
    , LoopEndFrame(0)
    , CurrentKey(0)
    , BlendAlpha(0.f)
    , BlendStartTime(0.f)
    , BlendDuration(0.2f)
    , bIsBlending(false)
{
}

void ULuaScriptAnimInstance::InitializeAnimation()
{
    UAnimInstance::InitializeAnimation();
    
    StateMachine = FObjectFactory::ConstructObject<UAnimStateMachine>(this);
    StateMachine->Initialize(Cast<USkeletalMeshComponent>(GetOuter()), this);
}

void ULuaScriptAnimInstance::NativeInitializeAnimation()
{
}

void ULuaScriptAnimInstance::NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
    UAnimInstance::NativeUpdateAnimation(DeltaSeconds, OutPose);
    StateMachine->ProcessState(DeltaSeconds);
    
#pragma region MyAnim
    USkeletalMeshComponent* SkeletalMeshComp = GetSkelMeshComponent();
    
    if (!PrevAnim || !CurrAnim || !SkeletalMeshComp->GetSkeletalMeshAsset() || !SkeletalMeshComp->GetSkeletalMeshAsset()->GetSkeleton() || !bPlaying)
    {
        return;
    }

    PreviousTime = ElapsedTime;
    PreElapsedTime += DeltaSeconds * PrePlayRate;
    ElapsedTime += DeltaSeconds * PlayRate;

    CurrAnim->EvaluateAnimNotifies(CurrAnim->Notifies, ElapsedTime, PreviousTime, DeltaSeconds, SkeletalMeshComp, CurrAnim, bLooping);

    if (CurrAnim && !bLooping)
    {
        const float AnimDuration = CurrAnim->GetDuration();

        ElapsedTime = FMath::Clamp(
            ElapsedTime,
            0.0f,
            FMath::FloorToFloat(AnimDuration * 10000) / 10000
        );
    }

    if (bIsBlending)
    {
        float BlendElapsed = ElapsedTime - BlendStartTime;
        BlendAlpha = FMath::Clamp(BlendElapsed / BlendDuration, 0.f, 1.f);

        if (BlendAlpha >= 1.f)
        {
            bIsBlending = false;
            PrevAnim = CurrAnim;
        }
    }
    else
    {
        BlendAlpha = 1.f;
    }
    
    // TODO: FPoseContext의 BoneContainer로 바꾸기
    const FReferenceSkeleton& RefSkeleton = this->GetCurrentSkeleton()->GetReferenceSkeleton();
    
    if (PrevAnim->GetSkeleton()->GetReferenceSkeleton().GetRawBoneNum()!= RefSkeleton.RawRefBoneInfo.Num() || CurrAnim->GetSkeleton()->GetReferenceSkeleton().GetRawBoneNum() != RefSkeleton.RawRefBoneInfo.Num())
    {
        return;
    }
    
    FPoseContext PrevPose(this);
    FPoseContext CurrPose(this);
    
    PrevPose.Pose.InitBones(RefSkeleton.RawRefBoneInfo.Num());
    CurrPose.Pose.InitBones(RefSkeleton.RawRefBoneInfo.Num());
    for (int32 BoneIdx = 0; BoneIdx < RefSkeleton.RawRefBoneInfo.Num(); ++BoneIdx)
    {
        PrevPose.Pose[BoneIdx] = RefSkeleton.RawRefBonePose[BoneIdx];
        CurrPose.Pose[BoneIdx] = RefSkeleton.RawRefBonePose[BoneIdx];
    }
    
    FAnimExtractContext ExtractA(PreElapsedTime, bPrevLooping);
    FAnimExtractContext ExtractB(ElapsedTime, bLooping);

    PrevAnim->GetAnimationPose(PrevPose, ExtractA);
    CurrAnim->GetAnimationPose(CurrPose, ExtractB);

    FAnimationRuntime::BlendTwoPosesTogether(CurrPose.Pose, PrevPose.Pose, BlendAlpha, OutPose.Pose);
#pragma endregion
}

void ULuaScriptAnimInstance::SetAnimation(UAnimSequence* NewAnim, float BlendingTime, float LoopAnim, bool ReverseAnim)
{
    if (CurrAnim == NewAnim)
    {
        return; // 이미 같은 애니메이션이 설정되어 있다면 아무 작업도 하지 않음.
    }

    if (!CurrAnim)
    {
        CurrAnim = NewAnim;
    }

    if (!PrevAnim)
    {
        PrevAnim = CurrAnim; // 이전 애니메이션이 없으면 현재 애니메이션을 이전으로 설정.
    }

    PreElapsedTime = ElapsedTime;
    ElapsedTime = 0.0f; // 현재 애니메이션이 있으면 시간 초기화.

    bPrevLooping = bLooping;
    bLooping = LoopAnim;
    
    PrevAnim = CurrAnim; // 현재 애니메이션이 있으면 현재를 이전으로 설정.
    CurrAnim = NewAnim;
    BlendDuration = BlendingTime;
    bReverse = ReverseAnim;
    
    //ElapsedTime = 0.0f;
    BlendStartTime = ElapsedTime;
    BlendAlpha = 0.0f;
    bIsBlending = true;
    bPlaying = true;
}
