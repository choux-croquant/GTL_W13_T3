#pragma once

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "BehellaScreenUI.h"

enum class EBehellaGameState 
{
    Ready,
    PrepareToPlay,
    Play,
    PlayToDead,
    Dead,
    DeadToGameOver,
    PlayToGameOver,
    GameOver,
    GameOverToPlay,
};

class FBehellaScreenUI;
class FBehellaGameInitScreenUI;
class FBehellaGamePlayScreenUI;
class FBehellaGameDeadScreenUI;
class FBehellaGameOverScreenUI;

class AHeroPlayer;
class AEnemy;

class ABehellaGameMode : public AGameMode 
{
    DECLARE_CLASS(ABehellaGameMode, AGameMode)

public:
    ABehellaGameMode() = default;
    virtual ~ABehellaGameMode() override = default;

    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;

    virtual UObject* Duplicate(UObject* InOuter) override;

    void CloseScreen(FBehellaScreenUI* Screen);

    // 게임 모드 초기화
    virtual void InitGame() override;

    // 게임 시작 넘어가는 순간
    virtual void PrepareMatch();

    // 게임 시작
    virtual void StartMatch() override;

    // 플레이어 승리
    void PlayerWin();

    // 플레이어 패배(사망)
    void PlayerDead();

    // 게임 종료
    virtual void EndMatch(bool bIsWin) override;

    void ResetValue();

    void CheckFatality();

    virtual void Tick(float DeltaTime) override;

    
    static EBehellaGameState GameState;
  
    float StepTimer = 0.0f;

    FBehellaScreenUI* CurScreenUI = nullptr;
    FBehellaScreenUI* ClosingScreenUI = nullptr;

    FBehellaGameInitScreenUI InitScreenUI;
    FBehellaGamePlayScreenUI PlayScreenUI;
    FBehellaGameDeadScreenUI DeadScreenUI;
    FBehellaGameOverScreenUI GameOverScreenUI;

    AHeroPlayer* HeroPlayer = nullptr;
    AEnemy* Enemy = nullptr;

    const float MaxParryGauge = 100.f;
    const float ClosingUITime = 1.0f;
};
