#include "AnimStateMachine.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/Contents/AnimInstance/LuaScriptAnimInstance.h"
#include "Lua/LuaScriptManager.h"
#include "Animation/AnimSequence.h"

UAnimStateMachine::UAnimStateMachine()
{
    
}

void UAnimStateMachine::Initialize(USkeletalMeshComponent* InOwner, ULuaScriptAnimInstance* InAnimInstance)
{
    OwningComponent = InOwner;
    OwningAnimInstance = InAnimInstance;

    LuaScriptName = OwningComponent->StateMachineFileName;
    InitLuaStateMachine();
}

void UAnimStateMachine::ChangeStateMachineLua(FString InState)
{
    State = InState;

    if (!LuaTable.valid())
        return;
    
    sol::function ChangeStateMachineFunc = LuaTable["TransitionToState"];
    if (ChangeStateMachineFunc.valid())
    {
        ChangeStateMachineFunc(LuaTable, *State);
    }
}

FString UAnimStateMachine::GetState()
{
    return State;
}

void UAnimStateMachine::ProcessState(float DeltaTime)
{
    if (!LuaTable.valid())
        return;
    
    sol::function UpdateFunc = LuaTable["Update"];
    if (!UpdateFunc.valid())
    {
        UE_LOG(ELogLevel::Warning, TEXT("Lua Update function not valid!"));
        return;
    }
    sol::object result = UpdateFunc(LuaTable, DeltaTime);

    if (!result.valid())
    {
        return;
    }
    
    sol::table StateInfo = result.as<sol::table>();
    FString AnimName = StateInfo["anim"].get_or(std::string("")).c_str();
    float Blend = StateInfo["blend"].get_or(0.f);
    
    bool bLoop = StateInfo["loop"].get_or(true);
    float RateScale = StateInfo["rate_scale"].get_or(1.0f);
    State = StateInfo["state"].get_or(std::string("")).c_str();
    
    if (OwningAnimInstance && !AnimName.IsEmpty())
    {
        if (State != PreState)
        {
            UAnimSequence* NewAnim = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(AnimName));

            if (NewAnim)
            {
                // 애니메이션 속성 설정
                PreState = State;
                OwningAnimInstance->SetAnimation(NewAnim, Blend, bLoop, false);
                OwningAnimInstance->SetPlayRate(RateScale);
                LuaTable["CurrentAnimDuration"] = NewAnim->GetDuration();
            }
        }
    }
}

void UAnimStateMachine::InitLuaStateMachine()
{
    if (LuaScriptName.IsEmpty())
    {
        return;
    }

    LuaTable = FLuaScriptManager::Get().CreateLuaTable(LuaScriptName);

    FLuaScriptManager::Get().RegisterActiveAnimLua(this);
    if (!LuaTable.valid())
        return;

    LuaTable["OwnerCharacter"] = Cast<AActor>(OwningComponent->GetOwner());
    LuaTable["CurrentAnimDuration"] = 0.0f;
}

