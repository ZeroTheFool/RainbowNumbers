// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "ColorEnum.h"
#include "CollectorManager.generated.h"

UCLASS()
class RAINBOWNUMBERS_API ACollectorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectorManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectors")
	TMap<ERainbowColor, TObjectPtr<AActor>> AllCollectors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* GetCollectorByColor(ERainbowColor color);
};
