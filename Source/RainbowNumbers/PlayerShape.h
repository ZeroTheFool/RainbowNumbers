// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PoolableObjectComponent.h"
#include "PlayerShape.generated.h"

class AProjectile;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class RAINBOWNUMBERS_API APlayerShape : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerShape();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	TObjectPtr<UStaticMeshComponent> PlayerBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	TObjectPtr<UBoxComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	TObjectPtr<UPoolableObjectComponent> PoolComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UPROPERTY()
	TMap<ERainbowColor, int32> OrbCollection;

	UPROPERTY()
	TMap<ERainbowColor, ERainbowColor> SecondaryColorPairs;

	UPROPERTY(EditAnywhere, Category = "Projectile Spawner")
	TSubclassOf<AProjectile> ProjectileClass;

	UFUNCTION()
	void AddOrb(ERainbowColor color);

	void CheckColorCombinations();

	void CheckAttributeChanges();

	float CalculateSecondaryColor(ERainbowColor first, ERainbowColor second);

	void OnProjectileCooldown();

	FTimerHandle FiringCooldown_Timer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReceiveDamage(float inDamage);

	float GetDamageValue() { return BaseDamage; }

	int32 CheckWhiteScore() { return WhiteScore; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float BaseHealth = 50.0f; // red

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth; // red

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float HealthPerOrb = 5.0f; // red

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseDamage = 1.0f; // yellow

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float DamagePerOrb = 1.0f; // yellow

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentDamage = 1.0f; // yellow

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseSpeed = 10.0f; // blue

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float SpeedPerOrb = 1.0f; // blue

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentSpeed = 10.0f; // blue

	// green - enemy spawn rate

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseShootingCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float CurrentShootingCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float ShootingCooldownPerOrb = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 ProjectileCount = 0; // purple

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float ProjectileLifeTime = 1.0f; // green

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BaseProjectileLifeTime = 1.0f; // green

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float ProjectileLifeTimePerOrb = 0.5f; // green

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FVector PlayerScale = FVector(1.0f); // green

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FVector PlayerScalePerOrb = FVector(0.1f); // green

	// UpgradeRates
	TMap<ERainbowColor, float> UpgradeRates;

	int32 WhiteScore = 0;




	FVector2D PreviousMousePosition;


};
