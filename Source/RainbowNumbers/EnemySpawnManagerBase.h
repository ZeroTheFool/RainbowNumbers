// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolableObjectComponent.h"
#include "EnemySpawnManagerBase.generated.h"

class AEnemyShapeBase;

UCLASS()
class RAINBOWNUMBERS_API AEnemySpawnManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnManagerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	TObjectPtr<UPoolableObjectComponent> PoolComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Projectile Spawner")
	TSubclassOf<AEnemyShapeBase> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawner")
	float SpawnDelaySeconds = 2.0f;

	FTimerHandle EnemySpawnDelay_Timer;

	void OnEnemySpawnDelay();

	int SpawnedEnemies = 0;

	int MaxEnemiesPerSpeedIncrease = 15;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawner")
	bool bShouldSpawnPeriodically = true;

	UFUNCTION()
	void SetSpawningEnemies(bool shouldSpawn);
};
