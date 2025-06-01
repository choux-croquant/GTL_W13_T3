#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"

void AEnemy::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    SkeletalMeshComponent = AddComponent<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetSkeletalMeshAsset(UAssetManager::Get().GetSkeletalMesh(FName("Contents/GameJamEnemy/GameJamEnemy")));
    SkeletalMeshComponent->StateMachineFileName = TEXT("LuaScripts/Animations/EnemyStateMachine.lua");
    SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    SkeletalMeshComponent->SetAnimClass(UClass::FindClass(FName("ULuaScriptAnimInstance")));
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
