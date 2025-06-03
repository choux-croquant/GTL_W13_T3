#include "BehellaGameMode.h"

void ABehellaGameMode::PostSpawnInitialize()
{
    AGameMode::PostSpawnInitialize();

}

UObject* ABehellaGameMode::Duplicate(UObject* InOuter)
{
    ABehellaGameMode* NewActor = Cast<ABehellaGameMode>(Super::Duplicate(InOuter));


    return NewActor;
}

void ABehellaGameMode::InitGame()
{
    Super::InitGame();  // Super에서 Delegate BroadCast 걸어줌

    // 게임 State 설정
    GameState = EBehellaGameState::Prepare;
    // 게임 초기 화면 UI 키기
    CurScreenUI = &InitScreenUI;
    CurScreenUI->InitScreen();

    // 캐릭터에 대한 Input 차단 명령
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^ TODO
    
}

void ABehellaGameMode::StartMatch()
{
    Super::StartMatch();   //  Super에서 Delegate BroadCast 걸어줌

    // 게임 State 설정

    // 게임 Play 화면 UI 켜기

    // 캐릭터에 대한 Input 차단 풀기

}

void ABehellaGameMode::EndMatch(bool bIsWin)
{
    // 게임 State 설정

    // 게임 End 화면 UI 켜기

    // 캐릭터에 대한 Input 차단 걸기

    Super::EndMatch(bIsWin);  //  Super에서 Delegate BroadCast 걸어줌
}

void ABehellaGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CurScreenUI->TickScreen(DeltaTime);

    StepTimer += DeltaTime;
}
