// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolableObject.h"
#include "ColorEnum.h"
#include "EnemyShapeBase.generated.h"

class AExperienceOrbBase;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class RAINBOWNUMBERS_API AEnemyShapeBase : public APoolableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyShapeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Body")
	TObjectPtr<UBoxComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Body")
	TObjectPtr<UStaticMeshComponent> EnemyBody;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetInUse(bool inUse);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChasePlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed = 500.0f;

	FVector CurrentDirection = FVector(0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUpgradeOverTime = false;

	FVector StartLocation;
	FVector TargetLocation;
	float ElapsedTravelTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpDuration = 5.0f;

	FVector TravelDirection = FVector(0.0f); // unit vector


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReceiveDamage(float inDamage);

	ERainbowColor CheckColor() { return EnemyColor; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERainbowColor EnemyColor = ERainbowColor::None;

	UPROPERTY(EditAnywhere, Category = "Orb Spawner")
	TSubclassOf<AExperienceOrbBase> OrbClass;

	FTimerHandle ContinuousDamage_Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDelay = 1.0f;
	
	float DamageValueStored = 0.0f;

	void OnContinuousDamage();

	UPROPERTY(EditAnywhere, Category = "Color Merge")
	bool bCanMerge = false;

	bool bIsMergingNow = false;

	void MergeAndSpawnNew(AActor* otherActor, ERainbowColor color);
};
