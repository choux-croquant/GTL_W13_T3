#pragma once

#include "LuaScripts/LuaUIManager.h"

class FBehellaScreenUI
{
public:
    virtual void InitScreen();  // 시작되는 Screen 임을 의미, UI 페이드 인 로직 넣어주기

    virtual void TickScreen(float DeltaTime);

    virtual void CloseScreen(float InClosingTime); // 이제 꺼지는 Screen 임을 의미, UI 투명화 로직 넣어주기

    virtual void EndScreen();   // 완전히 UI 요소 제거

    float UIStepTimer = 0.0f;
    float ClosingTimer = 0.0f;
    float ClosingTime = 1.0f;
    bool bClosing = false;
};

class FBehellaGameInitScreenUI : public FBehellaScreenUI
{
public:
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;  // TODO 부모 클래스 EndScreen 호출 필요

    LuaImageUI* FadeImage;
    LuaImageUI* LogoImage;
    LuaImageUI* PressKeyImage;

    float FadeContinueTime = 1.0f;
    float FadeOutTime = 1.0f;
    float LogoFadeTime = 1.0f;
    float PressKeyRevealTime = 1.5f;
    float PressKeyLoopTime = 4.0f;
};


class FBehellaGamePlayScreenUI : public FBehellaScreenUI
{
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;

    LuaTextUI* ScreenName;
};

class FBehellaGameDeadScreenUI : public FBehellaScreenUI
{
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;

    LuaTextUI* ScreenName;
};

class FBehellaGameOverScreenUI : public FBehellaScreenUI
{
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;

    LuaTextUI* ScreenName;
};
