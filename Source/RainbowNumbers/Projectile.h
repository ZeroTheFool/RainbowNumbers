// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolableObject.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class RAINBOWNUMBERS_API AProjectile : public APoolableObject
{
	GENERATED_BODY()

	/** Provides collision detection for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Root;

	/** Provides collision detection for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ProjectileBody;

	/** Handles movement for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	float BaseDamage = 1.0f;

	float ProjectileSpeed = 500.0f;

	FTimerHandle OnLifeTimeReached_Timer;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void SetDamageValue(float inDamage) { BaseDamage = inDamage; }

	void SetInUse(bool inUse);


};
