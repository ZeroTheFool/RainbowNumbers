// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShapeBase.h"
#include "PlayerShape.h"
#include "ExperienceOrbBase.h"
#include "ExperienceOrbSpawner.h"
#include "EnemySpawnerManager.h"
#include "EnemySpawnManagerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyShapeBase::AEnemyShapeBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<UBoxComponent>("Root");
    Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Root->SetCollisionProfileName("OverlapAll"); // needs to ignore everything but player
    Root->SetGenerateOverlapEvents(true);
    Root->OnComponentBeginOverlap.AddDynamic(this, &AEnemyShapeBase::OnOverlapBegin);
    SetRootComponent(Root);

    EnemyBody = CreateDefaultSubobject<UStaticMeshComponent>("Enemy");
    EnemyBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyBody->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AEnemyShapeBase::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
}

// Called every frame
void AEnemyShapeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();

    CurrentLocation = FMath::VInterpConstantTo(
        CurrentLocation,
        TargetLocation,
        DeltaTime,
        BaseSpeed
    );

    SetActorLocation(CurrentLocation);

    if ((TargetLocation - CurrentLocation).Length() < 20.0f)
    {
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShape::StaticClass());
        // reached target, pick new direction
        if (FoundActor)
        {
            if (bChasePlayer)
            {
                TargetLocation = FoundActor->GetActorLocation();
                TravelDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
            }
            else
            {
                TravelDirection = FMath::VRand();
                TravelDirection.Z = 0.0f;
                TargetLocation = CurrentLocation + (TravelDirection * 1500.0f);
            }
        }
    }

    //float Remaining = GetWorldTimerManager().GetTimerRemaining(LifeTime_Timer);
}

void AEnemyShapeBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (OtherActor)
    {
        APlayerShape* Player = Cast<APlayerShape>(OtherActor);
        if (Player)
        {
            // this is to take and deal damage
            Player->ReceiveDamage(Damage);

            // get direction away from player??
            FVector DirectionToPlayer = GetActorLocation() - Player->GetActorLocation();
            DirectionToPlayer.Normalize();
            TargetLocation = GetActorLocation() + (DirectionToPlayer * FVector(100.f, 100.f, 0.0f));

            DamageValueStored = Player->GetDamageValue();

            GetWorld()->GetTimerManager().ClearTimer(ContinuousDamage_Timer);

            GetWorld()->GetTimerManager().SetTimer(ContinuousDamage_Timer, this, &AEnemyShapeBase::OnContinuousDamage, DamageDelay, true);
        }
        else
        {
            AEnemyShapeBase* Enemy = Cast<AEnemyShapeBase>(OtherActor);
            if (Enemy)
            {
                if (bCanMerge && Enemy->bCanMerge && EnemyColor != Enemy->CheckColor() && !bIsMergingNow && !Enemy->bIsMergingNow)
                {
                    // all conditions are good for this enemy to merge with another
                    bIsMergingNow = true;
                    Enemy->bIsMergingNow = true;

                    if (EnemyColor == ERainbowColor::Red || Enemy->CheckColor() == ERainbowColor::Red
                        && EnemyColor == ERainbowColor::Yellow || Enemy->CheckColor() == ERainbowColor::Yellow)
                    {
                        // spawn orange
                        MergeAndSpawnNew(OtherActor, ERainbowColor::Orange);
                    }
                    else if (EnemyColor == ERainbowColor::Yellow || Enemy->CheckColor() == ERainbowColor::Yellow
                        && EnemyColor == ERainbowColor::Blue || Enemy->CheckColor() == ERainbowColor::Blue)
                    {
                        // spawn green
                        MergeAndSpawnNew(OtherActor, ERainbowColor::Green);
                    }
                    else if (EnemyColor == ERainbowColor::Red || Enemy->CheckColor() == ERainbowColor::Red
                        && EnemyColor == ERainbowColor::Blue || Enemy->CheckColor() == ERainbowColor::Blue)
                    {
                        // spawn violet
                        MergeAndSpawnNew(OtherActor, ERainbowColor::Violet);
                    }

                    Enemy->SetInUse(false);
                    SetInUse(false);
                }
            }
        }
    }
}

void AEnemyShapeBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        APlayerShape* Player = Cast<APlayerShape>(OtherActor);
        if (Player)
        {
            GetWorld()->GetTimerManager().ClearTimer(ContinuousDamage_Timer);
            DamageValueStored = 0.0f;
        }
    }
}

void AEnemyShapeBase::SetInUse(bool inUse)
{
    Super::SetInUse(inUse);

    GetWorld()->GetTimerManager().ClearTimer(ContinuousDamage_Timer);

    if (bInUse)
    {
        StartLocation = GetActorLocation();
        TargetLocation = GetActorLocation();

        Health = MaxHealth;

        bIsMergingNow = false;

        TravelDirection = (TargetLocation - StartLocation).GetSafeNormal();
    }
}

void AEnemyShapeBase::ReceiveDamage(float inDamage)
{
    Health -= inDamage;

    if (Health <= 0.0f)
    {
        // spawn orb
        FTransform OrbTransform = GetActorTransform();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
        SpawnParams.Owner = GetOwner();

        AExperienceOrbSpawner* FoundSpawner = Cast<AExperienceOrbSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AExperienceOrbSpawner::StaticClass()));
        if (FoundSpawner)
        {
            APoolableObject* NewOrb = FoundSpawner->PoolComponent->SpawnAvailableObject(OrbTransform);

            if (NewOrb)
            {
                Cast<AExperienceOrbBase>(NewOrb)->UpdateColor(EnemyColor);
            }
        }

        SetInUse(false);
    }
}

void AEnemyShapeBase::OnContinuousDamage()
{
    ReceiveDamage(DamageValueStored);
}

void AEnemyShapeBase::MergeAndSpawnNew(AActor* otherActor, ERainbowColor color)
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnerManager::StaticClass());
    if (FoundActor)
    {
        AActor* SpawnActor = Cast<AEnemySpawnerManager>(FoundActor)->GetSpawnerByColor(color);
        Cast<AEnemySpawnManagerBase>(SpawnActor)->PoolComponent->SpawnAvailableObject(GetActorTransform());
    }
}
