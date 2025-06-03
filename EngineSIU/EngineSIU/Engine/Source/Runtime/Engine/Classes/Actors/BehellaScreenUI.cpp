#include "BehellaScreenUI.h"

#include "Engine/Source/Developer/LuaUtils/LuaTextUI.h"
#include "Engine/Source/Developer/LuaUtils/LuaImageUI.h"
#include "Engine/Source/Developer/LuaUtils/LuaButtonUI.h"
#include "Engine/Source/Developer/LuaUtils/LuaSliderUI.h"

#include <cmath>

void FBehellaScreenUI::InitScreen()
{
    UIStepTimer = 0.0f;
}

void FBehellaScreenUI::TickScreen(float DeltaTime)
{
    UIStepTimer += DeltaTime;
    if (bClosing) 
    {
        ClosingTimer += DeltaTime;
    }
}

void FBehellaScreenUI::CloseScreen(float InClosingTime)
{
    ClosingTime = InClosingTime;
    bClosing = true;
}

void FBehellaScreenUI::EndScreen()
{
    UIStepTimer = 0.0f;
    ClosingTimer = 0.0f;
    bClosing = false;
}

void FBehellaGameInitScreenUI::InitScreen()
{
    FBehellaScreenUI::InitScreen();

    // Fade Image 처음에는 Black
    FadeImage = LuaUIManager::Get().CreateImage(FName("FadeBG")
        , RectTransform(0.0f, 0.0f, 50000.0f, 50000.0f, AnchorDirection::TopLeft, AnchorDirection::TopLeft)
        , -1, "WhiteBox", FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
    
    // 로고와 PressAnyKey 생성 처음 투명도는 0.0f;
    LogoImage = LuaUIManager::Get().CreateImage(FName("BehellaLogo")
        , RectTransform(0.0f, 0.0f, 512.0f, 512.0f, AnchorDirection::MiddleCenter, AnchorDirection::MiddleCenter)
        , 5, "BehellaLogo", FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));


    PressKeyImage = LuaUIManager::Get().CreateImage(FName("PressSpace")
        , RectTransform(0.0f, 400.0f, 256.0f, 256.0f, AnchorDirection::MiddleCenter, AnchorDirection::MiddleCenter)
        , 5, "PressSpace", FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
}

void FBehellaGameInitScreenUI::TickScreen(float DeltaTime)
{
    // 부모 클래스에서 UIStepTimer(경과 시간)를 증가시킵니다.
    FBehellaScreenUI::TickScreen(DeltaTime);

    if (!bClosing) 
    {
        // 단계별 시간 구간
        const float fadeEndTime = FadeContinueTime;                                  // 검은 화면 페이드 아웃 종료 시점
        const float logoStartTime = fadeEndTime;                                        // 로고 페이드 인 시작
        const float logoEndTime = fadeEndTime + LogoFadeTime;                         // 로고 페이드 인 종료
        const float pressRevealStart = logoEndTime;                                        // PressAnyKey 노출 대기 시작
        const float pressRevealEnd = pressRevealStart + PressKeyRevealTime;              // PressAnyKey 완전 노출 시작
        const float pressLoopPeriod = PressKeyLoopTime;                                   // PressAnyKey 깜빡임 한 사이클(0→1→0에 해당하는 sin 파라미터 구간)

        float t = UIStepTimer;

        // 1) FadeImage: 0 → FadeContinueTime 구간 동안
        if (t < fadeEndTime)
        {
            // 0 <= t < fadeEndTime: alpha = cos( (t / fadeEndTime) * (PI/2) ) 
            // → t=0일 때 cos(0)=1, t=fadeEndTime일 때 cos(pi/2)=0
            float alpha = std::cos((t / fadeEndTime) * (PI / 2.0f));
            FadeImage->SetColor(FLinearColor(0.0f, 0.0f, 0.0f, alpha));
            // 로고와 PressKey는 아직 투명하게 유지
            LogoImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
            PressKeyImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
            return;
        }

        // 2) FadeImage는 완전히 투명, 이제 로고 페이드 인 구간 (fadeEndTime <= t < logoEndTime)
        FadeImage->SetColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

        if (t < logoEndTime)
        {
            float localT = t - logoStartTime; // 0 ~ LogoFadeTime
            // 로고 alpha = sin( (localT / LogoFadeTime) * (PI/2) )
            float alpha = std::sin((localT / LogoFadeTime) * (PI / 2.0f));
            LogoImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, alpha));
            // PressKey는 아직 투명
            PressKeyImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
            return;
        }

        // 3) 로고 완전히 보이는 상태 (t >= logoEndTime)
        LogoImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

        // 4) PressAnyKey 노출 대기 구간 (logoEndTime <= t < pressRevealEnd)
        if (t < pressRevealEnd)
        {
            // 아직 PressKey 고정 투명
            PressKeyImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
            return;
        }

        // 5) PressAnyKey가 깜빡이는 구간 (t >= pressRevealEnd)
        {
            float blinkT = t - pressRevealEnd; // 0 이상
            // 한 사이클(0→1→0)을 위해 sin 파라미터를 0~π 구간에 매핑
            float phase = std::fmod(blinkT, pressLoopPeriod) / pressLoopPeriod; // 0 ~ 1
            float alpha = std::sin(phase * PI); // 0→1→0 (phase: 0→1)
            PressKeyImage->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, alpha));
        }
    }
    else // Closing UI인 경우
    {

    }

    
}

void FBehellaGameInitScreenUI::CloseScreen(float InClosingTime)
{
    FBehellaScreenUI::CloseScreen(InClosingTime);
}

void FBehellaGameInitScreenUI::EndScreen()
{
    // 각 UI들 Manager에 삭제 요청
    LuaUIManager::Get().DeleteUI(FadeImage->GetName());
    LuaUIManager::Get().DeleteUI(LogoImage->GetName());
    LuaUIManager::Get().DeleteUI(PressKeyImage->GetName());

    FBehellaScreenUI::EndScreen();
}

void FBehellaGamePlayScreenUI::InitScreen()
{
    FBehellaScreenUI::InitScreen();

    ScreenName = LuaUIManager::Get().CreateText(FName("PlayScreen")
        , RectTransform(0.0f, 400.0f, 256.0f, 256.0f, AnchorDirection::MiddleCenter, AnchorDirection::MiddleCenter)
        , 5, FString("PlayScreenUI"), FName("Default"), 50.0f, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
}

void FBehellaGamePlayScreenUI::TickScreen(float DeltaTime)
{
    FBehellaScreenUI::TickScreen(DeltaTime);

    if (!bClosing)
    {
    }
}

void FBehellaGamePlayScreenUI::CloseScreen(float InClosingTime)
{
    FBehellaScreenUI::CloseScreen(InClosingTime);
}

void FBehellaGamePlayScreenUI::EndScreen()
{
    FBehellaScreenUI::EndScreen();
}

void FBehellaGameDeadScreenUI::InitScreen()
{
    FBehellaScreenUI::InitScreen();

    ScreenName = LuaUIManager::Get().CreateText(FName("PlayScreen")
        , RectTransform(0.0f, 400.0f, 256.0f, 256.0f, AnchorDirection::MiddleCenter, AnchorDirection::MiddleCenter)
        , 5, FString("PlayScreenUI"), FName("Default"), 50.0f, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
}

void FBehellaGameDeadScreenUI::TickScreen(float DeltaTime)
{
    FBehellaScreenUI::TickScreen(DeltaTime);
}

void FBehellaGameDeadScreenUI::CloseScreen(float InClosingTime)
{
    FBehellaScreenUI::CloseScreen(InClosingTime);
}

void FBehellaGameDeadScreenUI::EndScreen()
{
    FBehellaScreenUI::EndScreen();
}

void FBehellaGameOverScreenUI::InitScreen()
{
    FBehellaScreenUI::InitScreen();

    ScreenName = LuaUIManager::Get().CreateText(FName("PlayScreen")
        , RectTransform(0.0f, 400.0f, 256.0f, 256.0f, AnchorDirection::MiddleCenter, AnchorDirection::MiddleCenter)
        , 5, FString("PlayScreenUI"), FName("Default"), 50.0f, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
}

void FBehellaGameOverScreenUI::TickScreen(float DeltaTime)
{
    FBehellaScreenUI::TickScreen(DeltaTime);
}

void FBehellaGameOverScreenUI::CloseScreen(float InClosingTime)
{
    FBehellaScreenUI::CloseScreen(InClosingTime);
}

void FBehellaGameOverScreenUI::EndScreen()
{
    FBehellaScreenUI::EndScreen();
}
