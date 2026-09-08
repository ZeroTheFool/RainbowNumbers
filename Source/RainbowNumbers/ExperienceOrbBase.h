// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolableObject.h"
#include "ColorEnum.h"
#include "ExperienceOrbBase.generated.h"

class ExperienceCollector;
class USphereComponent;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class RAINBOWNUMBERS_API AExperienceOrbBase : public APoolableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExperienceOrbBase();

	UPROPERTY(EditAnywhere, Category = "Orb Body")
	TObjectPtr<UStaticMeshComponent> OrbBody;

	UPROPERTY(EditAnywhere, Category = "Orb Body")
	TObjectPtr<USphereComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInterface> CustomMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	ERainbowColor OrbColor = ERainbowColor::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	TMap<ERainbowColor, UMaterialInstance*> MaterialList;

	TObjectPtr<AActor> CorrespondingCollector = nullptr;

	bool bIsCollected = false;
	FVector StartLocation;
	FVector CollectorLocation;
	float ElapsedTravelTime;
	float LerpDuration = 2.0f;
	float BaseSpeed = 2000.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateColor(ERainbowColor color);

	ERainbowColor CheckColor() { return OrbColor; }
	bool CheckIsCollected() { return bIsCollected; }

	void SetCollected(bool isCollected = true);

	void SetInUse(bool inUse) override;

};
