#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SocketComponent.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"
#include "Animation/AnimCustomNotify.h"
#include "Animation/AnimSoundNotify.h"
#include "Engine/Contents/AnimInstance/LuaScriptAnimInstance.h"
#include "Actors/Player.h"
#include "UObject/UObjectIterator.h"
#include "Engine/SkeletalMesh.h"
#include "Userinterface/Console.h"
#include "Engine/Engine.h"
#include "Physics/PhysicsManager.h"

void AEnemy::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    // SetActorTickInEditor(true);
    SkeletalMeshComponent = AddComponent<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetSkeletalMeshAsset(UAssetManager::Get().GetSkeletalMesh(FName("Contents/GameJamEnemy/GameJamEnemy")));
    SkeletalMeshComponent->StateMachineFileName = TEXT("LuaScripts/Animations/EnemyStateMachine.lua");
    SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    SkeletalMeshComponent->SetAnimClass(UClass::FindClass(FName("ULuaScriptAnimInstance")));
    
    UObject* AssetObject = UAssetManager::Get().GetAsset(EAssetType::PhysicsAsset, FName("Contents/PhysicsAsset/UPhysicsAsset_212"));
    
    if (UPhysicsAsset* PhysicsAsset = Cast<UPhysicsAsset>(AssetObject))
    {
        SkeletalMeshComponent->GetSkeletalMeshAsset()->SetPhysicsAsset(PhysicsAsset);
    }
    
    SkeletalMeshComponent->bSimulate = true;

    UAnimSequence* IdleAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Idle/Armature|Enemy_Idle")));
    UAnimSequence* ReactionAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Impact/Armature|Enemy_Impact")));

    UAnimSequence* Horizontal1 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Horizontal1/Armature|Horizontal1")));
    UAnimSequence* Horizontal2 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Horizontal2/Armature|Horizontal2")));
    UAnimSequence* Vertical1 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Vertical1/Armature|Vertical1")));
    //  일단 여기서 초기화 하도록 함
    IdleAnim->RemoveAllNotifyTracks();
    Horizontal1->RemoveAllNotifyTracks();
    Horizontal2->RemoveAllNotifyTracks();
    Vertical1->RemoveAllNotifyTracks();
    ReactionAnim->RemoveAllNotifyTracks();

    // AttackNofity - Start
    CreateAttackNotify(IdleAnim, AttackToIdleNotify, "Attack_To_Idle", 0.0f);
    CreateAttackNotify(ReactionAnim, AttackToIdleNotify, "Attack_To_Idle", 0.0f);
    CreateAttackNotify(Horizontal1, AttackHorizontalNotify, "Attack_Horizontal", 0.0f);
    CreateAttackNotify(Horizontal2, AttackHorizontalNotify, "Attack_Horizontal", 0.0f);
    CreateAttackNotify(Vertical1, AttackVerticalNotify, "Attack_Vertical", 0.0f);

    CreateAttackNotify(Horizontal1, AttackHorizontalNotifyEnd, "Attack_Horizontal_End", Horizontal1->GetDuration() - 0.5f);
    CreateAttackNotify(Horizontal2, AttackHorizontalNotifyEnd, "Attack_Horizontal_End", Horizontal2->GetDuration() - 0.5f);
    CreateAttackNotify(Vertical1, AttackVerticalNotifyEnd, "Attack_Vertical_End", Vertical1->GetDuration() - 1.f);

    // Sound Notify
    // 패링 성공 시 - 피격 시작할 때 Notify
    CreateSoundNotify(ReactionAnim, ReactionNotify, "Impact", "Parry", 0.0f);
    // 공격 시도 시 사운드
    CreateSoundNotify(Horizontal1, PlayerHitNotify, "Hit", "SwordSwipe", 0.3f);
    CreateSoundNotify(Horizontal2, PlayerHitNotify, "Hit", "SwordSwipe", 0.3f);
    CreateSoundNotify(Vertical1, PlayerHitNotify, "Hit", "SwordSwipe", 0.3f);
}
    
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    BindAttackNotifies();
    SkeletalMeshComponent->bSimulate = false;
    InitialTransform = RootComponent->GetComponentTransform();
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ParryGauge < 100.0f)
    {
        ParryGauge = FMath::Max(0.0f, ParryGauge - DeltaTime * 2.0f);
    }

    //UE_LOG(ELogLevel::Warning, TEXT("PARRY %f"), ParryGauge);
    //UE_LOG(ELogLevel::Warning, TEXT("ATTACK_DIRECTION %d"), CurrentAttackDirection);
}

UObject* AEnemy::Duplicate(UObject* InOuter)
{
    AEnemy* NewActor = Cast<AEnemy>(Super::Duplicate(InOuter));

    NewActor->SkeletalMeshComponent = NewActor->GetComponentByClass<USkeletalMeshComponent>();
    NewActor->ParryGauge = 0.0f;
    
    NewActor->AttackToIdleNotify = AttackToIdleNotify;
    NewActor->AttackVerticalNotify = AttackVerticalNotify;
    NewActor->AttackHorizontalNotify = AttackHorizontalNotify;
    NewActor->AttackVerticalNotifyEnd = AttackVerticalNotifyEnd;
    NewActor->AttackHorizontalNotifyEnd = AttackHorizontalNotifyEnd;

    //NewActor->BindAttackNotifies();

    return NewActor;
}

void AEnemy::HandleAttackNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, EAttackDirection InAttackDirection)
{
    if (!MeshComp || !Animation)
    {
        UE_LOG(ELogLevel::Error, TEXT("Invalid MeshComp or Animation in HandleAttackNotify"));
        return;
    }

    switch (InAttackDirection)
    {
    case AD_Vertical:
        UE_LOG(ELogLevel::Display, TEXT("ENEMY_ATTACK_Vertical"));
        break;
    case AD_Horizontal:
        UE_LOG(ELogLevel::Display, TEXT("ENEMY_ATTACK_Horizontal"));
        break;
    case AD_None:
        UE_LOG(ELogLevel::Display, TEXT("ATTACK_TO_IDLE"));
        break;
    default:
        break;
    }

    CurrentAttackDirection = InAttackDirection;
}

void AEnemy::HandleAttackNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !Animation)
    {
        UE_LOG(ELogLevel::Error, TEXT("Invalid MeshComp or Animation in HandleAttackNotify"));
        return;
    }

    UE_LOG(ELogLevel::Display, TEXT("ENEMY_ATTACK_END"));

    for (auto It : TObjectRange<AHeroPlayer>())
    {
        It->GetDamaged(1.0f);
    }

    CurrentAttackDirection = AD_None;
}

void AEnemy::CreateAttackNotify(
    UAnimSequence* AnimSequence,
    UAnimCustomNotify*& OutNotify,
    const FString& NotifyName,
    float TriggerTime)
{
    if (!OutNotify)
    {
        OutNotify = FObjectFactory::ConstructObject<UAnimCustomNotify>(this);
    }

    int32 TrackIndex = INDEX_NONE;
    AnimSequence->AddNotifyTrack(NotifyName, TrackIndex);

    int32 NotifyEventIndex = INDEX_NONE;
    AnimSequence->AddNotifyEvent(
        TrackIndex,
        TriggerTime,
        0.0f,
        NotifyName,
        NotifyEventIndex
    );

    AnimSequence->GetNotifyEvent(NotifyEventIndex)->SetAnimNotify(OutNotify);
}

void AEnemy::CreateSoundNotify(
    UAnimSequence* AnimSequence,
    UAnimSoundNotify*& OutNotify,
    const FString& NotifyName,
    const FString& SoundName,
    float TriggerTime)
{
    OutNotify = FObjectFactory::ConstructObject<UAnimSoundNotify>(this);

    int32 TrackIndex = INDEX_NONE;
    AnimSequence->AddNotifyTrack(NotifyName, TrackIndex);

    int32 NotifyEventIndex = INDEX_NONE;
    AnimSequence->AddNotifyEvent(
        TrackIndex,
        TriggerTime,
        0.0f,
        NotifyName,
        NotifyEventIndex
    );
    
    OutNotify->SetSoundName(FName(SoundName));

    AnimSequence->GetNotifyEvent(NotifyEventIndex)->SetAnimNotify(OutNotify);
}

void AEnemy::BindAttackNotifies()
{
    if (AttackVerticalNotify)
    {
        AttackVerticalNotify->OnCustomNotify.AddLambda(
            [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
                this->HandleAttackNotify(MeshComp, Animation, EAttackDirection::AD_Vertical);
            }
        );
    }

    if (AttackHorizontalNotify)
    {
        AttackHorizontalNotify->OnCustomNotify.AddLambda(
            [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
                this->HandleAttackNotify(MeshComp, Animation, EAttackDirection::AD_Horizontal);
            }
        );
    }

    if (AttackVerticalNotifyEnd)
    {
        AttackVerticalNotifyEnd->OnCustomNotify.AddLambda(
            [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
                this->HandleAttackNotifyEnd(MeshComp, Animation);
            }
        );
    }

    if (AttackHorizontalNotifyEnd)
    {
        AttackHorizontalNotifyEnd->OnCustomNotify.AddLambda(
            [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
                this->HandleAttackNotifyEnd(MeshComp, Animation);
            }
        );
    }

    if (AttackToIdleNotify)
    {
        AttackToIdleNotify->OnCustomNotify.AddLambda(
            [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
                this->HandleAttackNotify(MeshComp, Animation, EAttackDirection::AD_None);
            }
        );
    }
}

void AEnemy::ResetEnemyProperties()
{
    SkeletalMeshComponent->ChangeRigidBodyFlag(ERigidBodyType::KINEMATIC);
    SkeletalMeshComponent->bSimulate = false;
    GEngine->PhysicsManager->GetScene(GetWorld())->fetchResults(true);
    
    ParryGauge = 0.0f;
    CurrentAttackDirection = AD_None;
    ULuaScriptAnimInstance* AnimInstance = Cast<ULuaScriptAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
    AnimInstance->GetStateMachine()->ChangeStateMachineLua(FString("Idle"));
    RootComponent->SetWorldTransform(InitialTransform);


}

void AEnemy::OnFinalScene()
{
    SetActorLocation(FVector(-1145, 0, 0));
    SetActorRotation(FRotator(0, 0, 0));
}
