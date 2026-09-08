// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnManagerBase.h"
#include "EnemyShapeBase.h"

// Sets default values
AEnemySpawnManagerBase::AEnemySpawnManagerBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PoolComponent = CreateDefaultSubobject<UPoolableObjectComponent>("Object Pool Component");
    PoolComponent->PoolSize = 20;
    PoolComponent->PoolObjectClass = EnemyClass;
}

// Called when the game starts or when spawned
void AEnemySpawnManagerBase::BeginPlay()
{
    Super::BeginPlay();

    SetSpawningEnemies(bShouldSpawnPeriodically);
}

// Called every frame
void AEnemySpawnManagerBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AEnemySpawnManagerBase::SetSpawningEnemies(bool shouldSpawn)
{
    bShouldSpawnPeriodically = shouldSpawn;

    if (bShouldSpawnPeriodically)
    {
        float randStart = FMath::FRandRange(0.0f, 2.0f);

        GetWorld()->GetTimerManager().SetTimer(EnemySpawnDelay_Timer, this, &AEnemySpawnManagerBase::OnEnemySpawnDelay, SpawnDelaySeconds, true, randStart);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(EnemySpawnDelay_Timer);
    }
}

void AEnemySpawnManagerBase::OnEnemySpawnDelay()
{

    int choice = FMath::Rand() % 3;


    FVector2D minmax = FVector2D(3000.0f, 1600.0f);

    float Y = FMath::FRandRange(-minmax.Y, minmax.Y);
    float X = FMath::FRandRange(-minmax.X, minmax.X);

    FVector Location;

    switch (choice)
    {
    case 0:
        Location = FVector(minmax.X, Y, 0.0f);
        break;
    case 1:
        Location = FVector(-minmax.X, Y, 0.0f);
        break;
    case 2:
        Location = FVector(X, minmax.Y, 0.0f);
        break;
    case 3:
        Location = FVector(X, -minmax.Y, 0.0f);
        break;
    }

    FTransform SpawnTransform = FTransform(FRotator(0.0f), Location, FVector(1.0f));

    APoolableObject* NewEnemy = PoolComponent->SpawnAvailableObject(SpawnTransform);
    if (NewEnemy)
    {
        SpawnedEnemies++;

        if (SpawnedEnemies >= MaxEnemiesPerSpeedIncrease)
        {
            SpawnedEnemies = 0;
            SpawnDelaySeconds -= 0.2;
            if (SpawnDelaySeconds < 0.5f)
            {
                SpawnDelaySeconds = 0.5f;
            }
        }
    }
}