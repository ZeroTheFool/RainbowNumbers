// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectorManager.h"
#include "ColorEnum.h"

// Sets default values
ACollectorManager::ACollectorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollectorManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* ACollectorManager::GetCollectorByColor(ERainbowColor color)
{
	return *(AllCollectors.Find(color));
}

