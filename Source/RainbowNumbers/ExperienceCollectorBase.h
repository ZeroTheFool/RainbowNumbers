// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorEnum.h"
#include "ExperienceCollectorBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class RAINBOWNUMBERS_API AExperienceCollectorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExperienceCollectorBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Collector")
	TObjectPtr<UBoxComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Collector")
	TObjectPtr<UStaticMeshComponent> CollectorBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orb Collector", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> OrbCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Collector")
	ERainbowColor CollectorColor = ERainbowColor::None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	int32 OrbCount = 0;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOrbCount(int32 count);

};
