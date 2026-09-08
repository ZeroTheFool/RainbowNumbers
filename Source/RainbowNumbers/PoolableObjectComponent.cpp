// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableObjectComponent.h"
#include "PoolableObject.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPoolableObjectComponent::UPoolableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPoolableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializePool();
}

void UPoolableObjectComponent::InitializePool()
{
	FTransform ObjectTransform = FTransform(FRotator(0.0f), FVector(0.0f));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnParams.Owner = GetOwner();

	APoolableObject* NewPooledObject;

	for (int i = 0; i < PoolSize; i++)
	{
		NewPooledObject = GetWorld()->SpawnActor<APoolableObject>(
			PoolObjectClass,
			ObjectTransform,
			SpawnParams);

		if (NewPooledObject)
		{
			NewPooledObject->SetInUse(false);
			ObjectPool.AddUnique(NewPooledObject);
		}
	}
}

APoolableObject* UPoolableObjectComponent::FindFirstAvailableObject()
{
	for (APoolableObject* object : ObjectPool)
	{
		if (!object->CheckInUse())
		{
			return object;
		}
	}

	return nullptr;
}

APoolableObject* UPoolableObjectComponent::SpawnAvailableObject(FTransform spawnTransform)
{
	APoolableObject* spawnObject = FindFirstAvailableObject();
	if (spawnObject)
	{
		spawnObject->SetActorTransform(spawnTransform);
		spawnObject->SetInUse(true);

		return spawnObject;
	}

	return nullptr;
}


// Called every frame
//void UPoolableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

