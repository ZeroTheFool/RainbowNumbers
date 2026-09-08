// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShape.h"
#include "EnemyShapeBase.h"
#include "ExperienceOrbBase.h"
#include "CollectorManager.h"
#include "ExperienceCollectorBase.h"
#include "PoolableObjectComponent.h"
#include "Projectile.h"
#include "ColorEnum.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"

// Sets default values
APlayerShape::APlayerShape()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<UBoxComponent>("Root");
    Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Root->SetCollisionProfileName("OverlapAll"); // needs to ignore everything but player
    Root->SetGenerateOverlapEvents(true);
    Root->OnComponentBeginOverlap.AddDynamic(this, &APlayerShape::OnOverlapBegin);
    SetRootComponent(Root);

    PlayerBody = CreateDefaultSubobject<UStaticMeshComponent>("Player");
    PlayerBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PlayerBody->SetupAttachment(Root);

    PoolComponent = CreateDefaultSubobject<UPoolableObjectComponent>("Object Pool Component");
    PoolComponent->PoolSize = 20;
    PoolComponent->PoolObjectClass = ProjectileClass;

    MaxHealth = BaseHealth;
    CurrentHealth = BaseHealth;

    CurrentDamage = BaseDamage;

    // how many orbs needed for an upgrade
    UpgradeRates.Add(ERainbowColor::Red, 1);
    UpgradeRates.Add(ERainbowColor::Yellow, 10);
    UpgradeRates.Add(ERainbowColor::Blue, 5);
    UpgradeRates.Add(ERainbowColor::Green, 1);
    UpgradeRates.Add(ERainbowColor::Orange, 1);
    UpgradeRates.Add(ERainbowColor::Violet, 1);

    OrbCollection.Add(ERainbowColor::Red, 0);
    OrbCollection.Add(ERainbowColor::Yellow, 0);
    OrbCollection.Add(ERainbowColor::Blue, 0);
    OrbCollection.Add(ERainbowColor::Green, 0);
    OrbCollection.Add(ERainbowColor::Orange, 0);
    OrbCollection.Add(ERainbowColor::Violet, 0);

}
void APlayerShape::AddOrb(ERainbowColor color)
{
    if (OrbCollection.Contains(color))
    {

        int32 currentCount = *(OrbCollection.Find(color));
        currentCount++;

        OrbCollection.Add(color, currentCount);

        // everything happens on 10s, so only do the function in this case
        if (currentCount % 10 == 0)
        {
            CheckAttributeChanges();
        }
        CheckColorCombinations();
    }
}

void APlayerShape::CheckColorCombinations()
{
    int32 totalOrange = CalculateSecondaryColor(ERainbowColor::Red, ERainbowColor::Yellow);
    OrbCollection.Add(ERainbowColor::Orange, totalOrange);


    int32 totalGreen = CalculateSecondaryColor(ERainbowColor::Blue, ERainbowColor::Yellow);
    OrbCollection.Add(ERainbowColor::Green, totalGreen);

    int32 totalViolet = CalculateSecondaryColor(ERainbowColor::Red, ERainbowColor::Blue);
    OrbCollection.Add(ERainbowColor::Violet, totalViolet);

    // update secondary colours (otherwise they wouldn't change)
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACollectorManager::StaticClass());
    ACollectorManager* CollectorManager = Cast<ACollectorManager>(FoundActor);
    if (CollectorManager)
    {
        Cast<AExperienceCollectorBase>(CollectorManager->GetCollectorByColor(ERainbowColor::Orange))->SetOrbCount(totalOrange);

        Cast<AExperienceCollectorBase>(CollectorManager->GetCollectorByColor(ERainbowColor::Green))->SetOrbCount(totalGreen);

        Cast<AExperienceCollectorBase>(CollectorManager->GetCollectorByColor(ERainbowColor::Violet))->SetOrbCount(totalViolet);

        Cast<AExperienceCollectorBase>(CollectorManager->GetCollectorByColor(ERainbowColor::White))->SetOrbCount(WhiteScore);
    }

}

void APlayerShape::CheckAttributeChanges()
{
    int32 fewestOfColor = 500;

    for (const auto& [Color, Value] : OrbCollection)
    {
        // this will find out how many white orbs are collected
        // 1 white for each full set of six colors, so find the lowest collected
        if (Value < fewestOfColor)
            fewestOfColor = Value;

        float actualMaxHealth;
        float checkCooldown;

        switch (Color)
        {
        case ERainbowColor::Red:
            // check what health should be
            // this determines how many upgrades there have been (upgrade rates)
            // and multiplies that value by the value per upgrade (_____PerOrb)
            actualMaxHealth = BaseHealth + ((Value / *(UpgradeRates.Find(Color))) * HealthPerOrb);
            // is the actual number higher than what is set?
            if (actualMaxHealth > MaxHealth)
            {
                // set the new value
                MaxHealth = actualMaxHealth;

                // just full heal
                CurrentHealth = MaxHealth;
            }
            break;
        case ERainbowColor::Orange:
            // how does this do nothing?
            // could this increase spawn speed?

            break;
        case ERainbowColor::Yellow:
            // ensure damage is what it should be
            CurrentDamage = BaseDamage + ((Value / *(UpgradeRates.Find(Color))) * DamagePerOrb);
            break;
        case ERainbowColor::Green:
            // projectile lifetime & cooldown
            ProjectileLifeTime = BaseProjectileLifeTime + ((Value / *(UpgradeRates.Find(Color))) * ProjectileLifeTimePerOrb);
            checkCooldown = BaseShootingCooldown + ((Value / *(UpgradeRates.Find(Color))) * 0.1f);
            if (CurrentShootingCooldown != checkCooldown)
            {
                GetWorld()->GetTimerManager().ClearTimer(FiringCooldown_Timer);
                GetWorld()->GetTimerManager().SetTimer(FiringCooldown_Timer, this, &APlayerShape::OnProjectileCooldown, BaseShootingCooldown, false);
            }
            CurrentShootingCooldown = checkCooldown;
            break;
        case ERainbowColor::Blue:
            // ensure speed is correct
            CurrentSpeed = BaseSpeed + ((Value / *(UpgradeRates.Find(Color))) * SpeedPerOrb);
            break;
        case ERainbowColor::Violet:
            // projectiles are 1:1 with violet colour
            ProjectileCount = Value;
            break;
        }
    }

    if (fewestOfColor != 0)
    {
        if (fewestOfColor > WhiteScore) // gained a white orb
        {
            // indicate with UI that white orb gained

            // add score
            WhiteScore = fewestOfColor;

            // check game over
        }
    }
}

float APlayerShape::CalculateSecondaryColor(ERainbowColor first, ERainbowColor second)
{
    int32 firstColor = *(OrbCollection.Find(first));
    int32 secondColor = *(OrbCollection.Find(second));

    // determine how many 10s of each have been collected
    int32 firstColorTens = firstColor / 10;
    int32 secondColorTens = secondColor / 10;

    // secondary color (orange) is equal to the lower of the two values
    return FMath::Min(firstColorTens, secondColorTens);


}

// Called when the game starts or when spawned
void APlayerShape::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(FiringCooldown_Timer, this, &APlayerShape::OnProjectileCooldown, BaseShootingCooldown, true);

}

// Called every frame
void APlayerShape::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APlayerShape::OnProjectileCooldown()
{
    CurrentShootingCooldown = BaseShootingCooldown;
    if (ProjectileCount > 0)
    {
        TArray<AActor*> HitActors;

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyShapeBase::StaticClass(), FoundActors);

        for (int i = 0; i < ProjectileCount; i++)
        {
            // find a target for each projectile
            AEnemyShapeBase* targetEnemy;
            int countAttempts = 0;
            do
            {
                int targetIndex = FMath::Rand() % FoundActors.Num();
                targetEnemy = Cast<AEnemyShapeBase>(FoundActors[targetIndex]);
                countAttempts++;
            } while (!targetEnemy->CheckInUse() && countAttempts < 30);

            if (targetEnemy)
            {
                if (HitActors.Find(targetEnemy) == INDEX_NONE)
                {
                    // only target each enemy once
                    HitActors.AddUnique(targetEnemy);

                    FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetEnemy->GetActorLocation());

                    FTransform ProjectileTransform = FTransform(ProjectileRotation, GetActorLocation(), FVector(1.0f));

                    //AProjectile* NewProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, ProjectileTransform);
                    APoolableObject* NewProjectile = PoolComponent->SpawnAvailableObject(ProjectileTransform);
                    if (NewProjectile)
                    {
                        // this sets the duration for the timer the projectile needs to set after it's been set
                        NewProjectile->SetLifeTime(ProjectileLifeTime);
                        // this seems like a dumb workaround, but reenabling it will clear and start the timer
                        NewProjectile->SetInUse(true);
                    }
                }
            }
        }
    }


}

// Called to bind functionality to input
void APlayerShape::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerShape::ReceiveDamage(float inDamage)
{
    CurrentHealth -= inDamage;

    if (CurrentHealth <= 0.0f)
    {
        Destroy();
    }
}

void APlayerShape::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (OtherActor)
    {
        AEnemyShapeBase* Enemy = Cast<AEnemyShapeBase>(OtherActor);
        if (Enemy)
        {
            // this is to take and deal damage
            Enemy->ReceiveDamage(CurrentDamage);
        }
        else
        {
            AExperienceOrbBase* Orb = Cast<AExperienceOrbBase>(OtherActor);
            if (Orb)
            {
                AddOrb(Orb->CheckColor());
                Orb->SetCollected();
            }
        }
    }
}