#include "PoolableObject.h"
#include "Engine/World.h"
#include "TimerManager.h"

void APoolableObject::BeginPlay()
{
    Super::BeginPlay();

    SetInUse(false);
}

void APoolableObject::OnLifeTimeReached()
{
    SetInUse(false);
}

void APoolableObject::SetInUse(bool inUse)
{
    bInUse = inUse;

    // set collision
    SetActorEnableCollision(bInUse);
    // set visibility (negate for visible)
    SetActorHiddenInGame(!bInUse);
    // set tick
    SetActorTickEnabled(bInUse);

    UE_LOG(LogTemp, Warning,
        TEXT("DEACTIVATE %s | Time=%.3f | TimerRemaining=%.3f"),
        *GetName(),
        GetWorld()->GetTimeSeconds(),
        GetWorld()->GetTimerManager().GetTimerRemaining(LifeTime_Timer));

    GetWorldTimerManager().ClearTimer(LifeTime_Timer);

    if (bInUse)
    {
        GetWorld()->GetTimerManager().SetTimer(
            LifeTime_Timer,
            this,
            &APoolableObject::OnLifeTimeReached,
            TimeUntilInactive,
            false);

        UE_LOG(LogTemp, Warning, TEXT("TimeToLive On Spawn: %f, %s"), TimeUntilInactive, *GetName());
    }
    else
    {
        //GetWorldTimerManager().ClearTimer(LifeTime_Timer);
    }
}
