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

    EnemyAttackNotify = FObjectFactory::ConstructObject<UAnimCustomNotify>(this);

    // Bind Delegate sample
    EnemyAttackNotify->OnCustomNotify.AddLambda(
        [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
        {
            this->HandleAttackNotify(MeshComp, Animation);
        }
    );

    // Apply Notify
    int32 TrackIndex = INDEX_NONE;
    Attack1->AddNotifyTrack("Enemy_Attack", TrackIndex);

    int32 NotifyEventIndex1 = INDEX_NONE;
    int32 NotifyEventIndex2 = INDEX_NONE;
    int32 NotifyEventIndex3 = INDEX_NONE;

    Attack1->AddNotifyEvent(TrackIndex, 0.2f, 0.0f, "Attack_Right", NotifyEventIndex1);
    Attack1->AddNotifyEvent(TrackIndex, 0.6f, 0.0f, "Attack_Light", NotifyEventIndex2);
    Attack1->AddNotifyEvent(TrackIndex, 1.0f, 0.0f, "Attack_Up", NotifyEventIndex3);

    Attack1->GetNotifyEvent(NotifyEventIndex1)->SetAnimNotify(EnemyAttackNotify);
    Attack1->GetNotifyEvent(NotifyEventIndex2)->SetAnimNotify(EnemyAttackNotify);
    Attack1->GetNotifyEvent(NotifyEventIndex3)->SetAnimNotify(EnemyAttackNotify);
}
    
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
}

UObject* AEnemy::Duplicate(UObject* InOuter)
{
    AEnemy* NewActor = Cast<AEnemy>(Super::Duplicate(InOuter));

    NewActor->SkeletalMeshComponent = NewActor->GetComponentByClass<USkeletalMeshComponent>();

    NewActor->EnemyAttackNotify = EnemyAttackNotify;
    //UAnimSequence* IdleAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Idle/Armature|Enemy_Idle")));
    //UAnimSequence* ReactionAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Enemy_Impact/Armature|Enemy_Impact")));
    //UAnimSequence* Attack1 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/Combo_RLU/Armature|Combo_RLU")));
    //UAnimSequence* Attack2 = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(FString("Contents/GameJamEnemy/Armature|Combo_RLR")));
    //
    //// AnimSequence는 Asset이라 PIE->Editor시 제거 안되어 일단 여기서 초기화 하도록 함
    //Attack1->RemoveNotifyTrack(0);

    //EnemyAttackNotify = FObjectFactory::ConstructObject<UAnimCustomNotify>(this);

    //// Bind Delegate sample
    //EnemyAttackNotify->OnCustomNotify.AddLambda(
    //    [this](USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
    //    {
    //        this->HandleAttackNotify(MeshComp, Animation);
    //    }
    //);

    //// Apply Notify
    //int32 TrackIndex = INDEX_NONE;
    //Attack1->AddNotifyTrack("Enemy_Attack", TrackIndex);

    //int32 NotifyEventIndex1 = INDEX_NONE;
    //int32 NotifyEventIndex2 = INDEX_NONE;
    //int32 NotifyEventIndex3 = INDEX_NONE;

    //Attack1->AddNotifyEvent(TrackIndex, 0.2f, 0.0f, "Attack_Right", NotifyEventIndex1);
    //Attack1->AddNotifyEvent(TrackIndex, 0.6f, 0.0f, "Attack_Light", NotifyEventIndex2);
    //Attack1->AddNotifyEvent(TrackIndex, 1.0f, 0.0f, "Attack_Up", NotifyEventIndex3);

    //Attack1->GetNotifyEvent(NotifyEventIndex1)->SetAnimNotify(EnemyAttackNotify);
    //Attack1->GetNotifyEvent(NotifyEventIndex2)->SetAnimNotify(EnemyAttackNotify);
    //Attack1->GetNotifyEvent(NotifyEventIndex3)->SetAnimNotify(EnemyAttackNotify);

    return NewActor;
}

void AEnemy::HandleAttackNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UE_LOG(ELogLevel::Display, TEXT("ENEMY_ATTACK"));
    //ExecuteAttack();
}
