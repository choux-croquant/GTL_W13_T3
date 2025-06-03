#pragma once

#include "Engine/Source/Runtime/CoreUObject/UObject/NameTypes.h"
#include "Engine/Source/Runtime/Core/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Container/Array.h"
#include "Engine/Source/ThirdParty/ImGui/include/ImGui/imgui.h"

enum AnchorDirection 
{
    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    MiddleCenter,
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight
};

struct RectTransform 
{
    FVector2D Position; // 로컬 위치
    FVector2D Size; // 너비 및 높이
    AnchorDirection AnchorDir;
    AnchorDirection AlignDir;   // 자체 정렬 어느 기준으로 할지

    RectTransform()
        : Position(0, 0), Size(100, 100), AnchorDir(AnchorDirection::MiddleCenter), AlignDir(AnchorDirection::MiddleCenter)
    { }

    RectTransform(float PosX, float PosY, float SizeX, float SizeY, AnchorDirection InAnchorDir, AnchorDirection InAlignDir)
        : Position(PosX, PosY), Size(SizeX, SizeY), AnchorDir(InAnchorDir), AlignDir(InAlignDir)
    { }

    ImVec2 GetAlignedPosition(const ImVec2& textSize) const
    {
        // Position은 이미 "Parent" → "Screen" 좌표계로 변환된 후라 가정
        float rectX = Position.X;
        float rectY = Position.Y;
        float rectW = textSize.x;
        float rectH = textSize.y;

        float x = rectX;
        float y = rectY;

        switch (AlignDir)
        {
        case TopLeft:
            x = rectX;
            y = rectY;
            break;
        case TopCenter:
            x = rectX - rectW * 0.5f;
            y = rectY;
            break;
        case TopRight:
            x = rectX - rectW;
            y = rectY;
            break;

        case MiddleLeft:
            x = rectX;
            y = rectY - rectH * 0.5f;
            break;
        case MiddleCenter:
            x = rectX - rectW * 0.5f;
            y = rectY - rectH * 0.5f;
            break;
        case MiddleRight:
            x = rectX - rectW;
            y = rectY - rectH * 0.5f;
            break;

        case BottomLeft:
            x = rectX;
            y = rectY - rectH;
            break;
        case BottomCenter:
            x = rectX - rectW * 0.5f;
            y = rectY - rectH;
            break;
        case BottomRight:
            x = rectX - rectW;
            y = rectY - rectH;
            break;
        }

        return ImVec2(x, y);
    }

    ImVec2 GetAlignedPosition() const
    {
        // Position은 이미 "Parent" → "Screen" 좌표계로 변환된 후라 가정
        float rectX = Position.X;
        float rectY = Position.Y;
        float rectW = Size.X;
        float rectH = Size.Y;

        float x = rectX;
        float y = rectY;

        switch (AlignDir)
        {
        case TopLeft:
            x = rectX;
            y = rectY;
            break;
        case TopCenter:
            x = rectX - rectW * 0.5f;
            y = rectY;
            break;
        case TopRight:
            x = rectX  - rectW;
            y = rectY;
            break;

        case MiddleLeft:
            x = rectX;
            y = rectY - rectH * 0.5f;
            break;
        case MiddleCenter:
            x = rectX - rectW * 0.5f;
            y = rectY - rectH * 0.5f;
            break;
        case MiddleRight:
            x = rectX - rectW;
            y = rectY - rectH * 0.5f;
            break;

        case BottomLeft:
            x = rectX;
            y = rectY - rectH;
            break;
        case BottomCenter:
            x = rectX - rectW * 0.5f;
            y = rectY - rectH;
            break;
        case BottomRight:
            x = rectX - rectW;
            y = rectY - rectH;
            break;
        }

        return ImVec2(x, y);
    }
};

class LuaUI 
{
public:
    LuaUI(FName InName) 
        : Name(InName), Visible(true)
    { }

protected:
    FName Name;
    bool Visible = false;
    RectTransform Rect;
    int SortOrder;  // SortOrder 값이 낮으면 먼저 렌더링됨

    LuaUI* ParentUI = nullptr;
    TArray<LuaUI*> ChildrenUI;

public:
    virtual void Create();
    FName GetName();
    FString GetNameStr();

    RectTransform& GetRectTransform() { return Rect; }

    void SetPosition(float X, float Y);
    void SetSize(float Width, float Height);
    void SetAnchorDir(AnchorDirection InAnchorDir);
    void SetSortOrder(int InSortOrder);
    
    virtual void DrawImGuiUI();

    RectTransform GetWorldRectTransform();
    int GetSortOrder() { return SortOrder; }
    bool GetVisible() { return Visible; }

    void SetParent(LuaUI* InParent);
    void AddChild(LuaUI* InChild);
};
