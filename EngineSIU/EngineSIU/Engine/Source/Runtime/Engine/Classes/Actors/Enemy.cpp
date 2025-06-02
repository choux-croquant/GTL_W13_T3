#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SocketComponent.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"
#include "Animation/AnimCustomNotify.h"
#include "Userinterface/Console.h"

void AEnemy::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    // SetActorTickInEditor(true);
    SkeletalMeshComponent = AddComponent<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetSkeletalMeshAsset(UAssetManager::Get().GetSkeletalMesh(FName("Contents/GameJamEnemy/GameJamEnemy")));
    SkeletalMeshComponent->StateMachineFileName = TEXT("LuaScripts/Animations/EnemyStateMachine.lua");
    SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    SkeletalMeshComponent->SetAnimClass(UClass::FindClass(FName("ULuaScriptAnimInstance")));

    UAnimSequence* IdleAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Idle/Armature|Enemy_Idle")));
    UAnimSequence* ReactionAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Impact/Armature|Enemy_Impact")));
    UAnimSequence* Attack1 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Combo_RLU/Armature|Combo_RLU")));
    UAnimSequence* Attack2 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/GameJamEnemy/Armature|Combo_RLR")));

    //  일단 여기서 초기화 하도록 함
    Attack1->RemoveNotifyTrack(0);

    CreateAttackNotify(Attack1, AttackHorizontalNotify, "Attack_Horizontal", 0.3f);
    CreateAttackNotify(Attack1, AttackHorizontalNotify, "Attack_Horizontal", 0.8f);
    CreateAttackNotify(Attack1, AttackVerticalNotify, "Attack_Vertical", 1.3f);

    BindAttackNotifies();
}
    
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
}

UObject* AEnemy::Duplicate(UObject* InOuter)
{
    AEnemy* NewActor = Cast<AEnemy>(Super::Duplicate(InOuter));

    NewActor->SkeletalMeshComponent = NewActor->GetComponentByClass<USkeletalMeshComponent>();

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
    default:
        break;
    }

    CurrentAttackDirection = InAttackDirection;
}

void AEnemy::CreateAttackNotify(
    UAnimSequence* AnimSequence,
    UAnimCustomNotify*& OutNotify,
    const FString& NotifyName,
    float TriggerTime)
{
    OutNotify = FObjectFactory::ConstructObject<UAnimCustomNotify>(this);

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
}
