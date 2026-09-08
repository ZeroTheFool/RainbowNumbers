// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerManager.h"

// Sets default values
AEnemySpawnerManager::AEnemySpawnerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AEnemySpawnerManager::GetSpawnerByColor(ERainbowColor color)
{
	return *(AllSpawners.Find(color));
}