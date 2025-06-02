#pragma once
#include<UObject/Object.h>
#include "UObject/ObjectMacros.h"

#include "sol/sol.hpp"

class USkeletalMeshComponent;
class ULuaScriptAnimInstance;
class APawn;

class UAnimStateMachine : public UObject
{
    DECLARE_CLASS(UAnimStateMachine, UObject)

public:
    UAnimStateMachine();
    virtual ~UAnimStateMachine() override = default;

    virtual void Initialize(USkeletalMeshComponent* InOwner, ULuaScriptAnimInstance* InAnimInstance);
    void ChangeStateMachineLua(FString InState);
    FString GetState();

    void ProcessState(float DeltaTime);
    
    void InitLuaStateMachine();
    
    FString GetLuaScriptName() const { return LuaScriptName; }

    USkeletalMeshComponent* OwningComponent;
    ULuaScriptAnimInstance* OwningAnimInstance;

    FString PreState = TEXT("");
private:
    UPROPERTY(EditAnywhere, FString, State, =TEXT("Idle"))
    UPROPERTY(EditAnywhere, FString, LuaScriptName, = TEXT(""))
    sol::table LuaTable = {};
};
