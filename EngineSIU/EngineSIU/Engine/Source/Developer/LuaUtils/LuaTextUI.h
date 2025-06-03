#pragma once

#include "LuaUI.h"
#include "Engine/Source/Runtime/Core/Container/String.h"
#include "Engine/Source/Runtime/Core/Math/Color.h"

class LuaTextUI : public LuaUI 
{
public:
    LuaTextUI(FName InName);
    LuaTextUI(FName InName, RectTransform InRectTransform, FString& InText, int InSortOrder, ImFont* InFontStyle, float InFontSize, FLinearColor InFontColor);

    virtual void DrawImGuiUI() override;

public:
    FString Text;
    ImFont* FontStyle;
    float FontSize;
    FLinearColor FontColor;

public:
    void SetText(FString& InText);
    void SetFont(ImFont* InFontStyle);
    void SetFontSize(float InFontSize);
    void SetFontColor(FLinearColor InFontColor);
};
