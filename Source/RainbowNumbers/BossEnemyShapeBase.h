// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEnemyShapeBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class RAINBOWNUMBERS_API ABossEnemyShapeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossEnemyShapeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Body")
	TObjectPtr<USphereComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Body")
	TObjectPtr<UStaticMeshComponent> EnemyBody;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ReceiveDamage(float inDamage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.0f;

	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	FVector TemporaryTargetLocation;

	FTimerHandle ContinuousDamage_Timer;
	void OnContinuousDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDelay = 1.0f;

	float DamageValueStored = 0.0f;

};
