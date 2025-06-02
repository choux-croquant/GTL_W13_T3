#pragma once
#include "ParticleModuleSizeBase.h"
#include "Distribution/DistributionVector.h"

class UParticleModuleSizeMultiplyLife : public UParticleModuleSizeBase
{
    DECLARE_CLASS(UParticleModuleSizeMultiplyLife, UParticleModuleSizeBase)

    UParticleModuleSizeMultiplyLife();

    virtual void DisplayProperty() override;
    
    virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;
    
    UPROPERTY_WITH_FLAGS(EditAnywhere, bool, MultiplyX)
    UPROPERTY_WITH_FLAGS(EditAnywhere, bool, MultiplyY)
    UPROPERTY_WITH_FLAGS(EditAnywhere, bool, MultiplyZ)

    UPROPERTY_WITH_FLAGS(EditAnywhere, bool, bUseConstantChange)
    UPROPERTY_WITH_FLAGS(EditAnywhere, bool, bUseSizeCurve)
    
    UPROPERTY_WITH_FLAGS(EditAnywhere, float, CurveScale)
    
    UPROPERTY_WITH_FLAGS(EditAnywhere, FVector, StartScale)
    UPROPERTY_WITH_FLAGS(EditAnywhere, FVector, EndScale)
    

    virtual void SerializeAsset(FArchive& Ar) override;
	//~ Begin UParticleModule Interface
};
