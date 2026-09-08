// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceOrbSpawner.h"
#include "ExperienceOrbBase.h"

// Sets default values
AExperienceOrbSpawner::AExperienceOrbSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoolComponent = CreateDefaultSubobject<UPoolableObjectComponent>("Object Pool Component");
	PoolComponent->PoolSize = 30;
	PoolComponent->PoolObjectClass = OrbClass;
}

// Called when the game starts or when spawned
void AExperienceOrbSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExperienceOrbSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

