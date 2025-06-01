#include "AnimCustomNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"

UAnimCustomNotify::UAnimCustomNotify()
{
}

void UAnimCustomNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnCustomNotify.Broadcast(MeshComp, Animation);
}
