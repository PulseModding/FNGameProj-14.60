#pragma once
#include "CoreMinimal.h"
#include "FortGameplayCueSpawnResult.generated.h"

class ADecalActor;
class AEmitterCameraLensEffectBase;
class UAudioComponent;
class UFXSystemComponent;
class UCameraShakeBase;

USTRUCT(BlueprintType)
struct FFortGameplayCueSpawnResult {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Transient, meta=(AllowPrivateAccess=true))
    TArray<UFXSystemComponent*> ParticleComponents;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Transient, meta=(AllowPrivateAccess=true))
    TArray<UAudioComponent*> AudioComponents;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, meta=(AllowPrivateAccess=true))
    UCameraShakeBase* CameraShake;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, meta=(AllowPrivateAccess=true))
    AEmitterCameraLensEffectBase* CameraLensEffect;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, meta=(AllowPrivateAccess=true))
    ADecalActor* DecalActor;
    
    FORTNITEGAME_API FFortGameplayCueSpawnResult();
};

