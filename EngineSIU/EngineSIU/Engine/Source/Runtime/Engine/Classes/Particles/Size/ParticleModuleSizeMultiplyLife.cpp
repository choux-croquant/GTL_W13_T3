#include "ParticleModuleSizeMultiplyLife.h"

#include "ParticleHelper.h"
#include "ParticleEmitterInstance.h"

UParticleModuleSizeMultiplyLife::UParticleModuleSizeMultiplyLife()
{
    bSpawnModule = true;
    bUpdateModule = true;

    MultiplyX = true;
    MultiplyY = true;
    MultiplyZ = true;
    
    bUseConstantChange = true;
    bUseSizeCurve = false;
    
    bUseUniformSize = true;

    CurveScale = 10.0f;

    StartScale = FVector::ZeroVector;
    EndScale = FVector::ZeroVector;
    
    ModuleName = "SizeMultiplyLife";
}

void UParticleModuleSizeMultiplyLife::DisplayProperty()
{
    Super::DisplayProperty();

    // 상호 배타적 옵션 동기화
    if (bUseConstantChange && bUseSizeCurve)
    {
        if (ImGui::IsItemEdited()) bUseSizeCurve = false;
    }
    else if (bUseSizeCurve && bUseConstantChange)
    {
        if (ImGui::IsItemEdited()) bUseConstantChange = false;
    }
    
    for (const auto& Property : StaticClass()->GetProperties())
    {
        ImGui::PushID(Property);
        Property->DisplayInImGui(this);
        ImGui::PopID();
    }
}

void UParticleModuleSizeMultiplyLife::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
    BEGIN_UPDATE_LOOP
    
    float RelTime = Particle.RelativeTime;

    FVector NewScale;
    if (bUseConstantChange)
    {
        float Alpha = FMath::Clamp(RelTime, 0.0f, 1.0f);    
        NewScale = FMath::Lerp(StartScale, EndScale, Alpha);
    }
    else if (bUseSizeCurve)
    {
        float t = FMath::Clamp(Particle.RelativeTime, 0.0f, 1.0f);
        float curveAlpha = 1.0f - FMath::Pow(1.0f - t, CurveScale);
        NewScale = FMath::Lerp(StartScale, EndScale, curveAlpha);
    }

    Particle.Size = Particle.BaseSize * NewScale;
    END_UPDATE_LOOP
}

void UParticleModuleSizeMultiplyLife::SerializeAsset(FArchive& Ar)
{
    Super::SerializeAsset(Ar);

    Ar << MultiplyX << MultiplyY << MultiplyZ << bUseConstantChange << bUseSizeCurve << CurveScale << StartScale << EndScale;
}
