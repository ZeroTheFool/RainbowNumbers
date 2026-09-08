#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "PoolableObject.generated.h"

/**
 *
 */

UCLASS(Blueprintable)
class RAINBOWNUMBERS_API APoolableObject : public AActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    
    bool bInUse = false;

    FTimerHandle LifeTime_Timer;

    void OnLifeTimeReached();

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings", meta = (ExposeOnSpawn = "true"))
    float TimeUntilInactive = 30.0f;

    virtual void SetInUse(bool inUse);

    virtual bool CheckInUse() { return bInUse; }

    virtual void SetLifeTime(float inTimeSeconds) { TimeUntilInactive = inTimeSeconds; }

};