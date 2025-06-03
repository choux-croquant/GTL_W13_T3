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

    float FadeAlpha = 0.0f;
    float LogoAlpha = 0.0f;
    float KeyAlpha = 0.0f;

    float FadeContinueTime = 1.0f;
    float FadeOutTime = 1.0f;
    float LogoFadeTime = 1.0f;
    float PressKeyRevealTime = 0.5f;
    float PressKeyLoopTime = 1.5f;
};


class FBehellaGamePlayScreenUI : public FBehellaScreenUI
{
public:
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;

    LuaTextUI* ScreenName;
    LuaSliderUI* ParryGaugeSlider;
    float ParryRatio = 0.0f; // 0~1

    float FadeInTime = 3.75f;
};

class FBehellaGameDeadScreenUI : public FBehellaScreenUI
{
public:
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;

    LuaTextUI* ScreenName;
    LuaImageUI* DiedImage;
    LuaImageUI* BGImage;

    float BGFadeInTime = 3.743f;
    float DiedFadeInTime = 1.743f;
};

class FBehellaGameOverScreenUI : public FBehellaScreenUI
{
public:
    virtual void InitScreen() override;

    virtual void TickScreen(float DeltaTime) override;

    virtual void CloseScreen(float InClosingTime) override;

    virtual void EndScreen() override;

    LuaTextUI* ScreenName;
};
