// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableObject.h"
#include "PoolableObjectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RAINBOWNUMBERS_API UPoolableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoolableObjectComponent();

	UPROPERTY(EditAnywhere, Category = "Spawn Class")
	TSubclassOf<APoolableObject> PoolObjectClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Class")
	int32 PoolSize = 50;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializePool();

	TArray<TObjectPtr<APoolableObject>> ObjectPool;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	APoolableObject* FindFirstAvailableObject();

	UFUNCTION(BlueprintCallable)
	APoolableObject* SpawnAvailableObject(FTransform spawnTransform);

protected:

};
