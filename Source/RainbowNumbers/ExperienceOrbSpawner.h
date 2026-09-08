// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolableObjectComponent.h"
#include "ExperienceOrbSpawner.generated.h"

class AExperienceOrbBase;

UCLASS()
class RAINBOWNUMBERS_API AExperienceOrbSpawner : public AActor
{
	GENERATED_BODY()
	


public:	
	// Sets default values for this actor's properties
	AExperienceOrbSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	TObjectPtr<UPoolableObjectComponent> PoolComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Projectile Spawner")
	TSubclassOf<AExperienceOrbBase> OrbClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
