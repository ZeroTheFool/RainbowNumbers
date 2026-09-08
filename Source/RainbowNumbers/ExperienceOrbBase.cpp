// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceOrbBase.h"
#include "CollectorManager.h"
#include "PlayerShape.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExperienceOrbBase::AExperienceOrbBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USphereComponent>("Root");
    Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Root->SetCollisionProfileName("OverlapAll"); // needs to ignore everything but player
    Root->SetGenerateOverlapEvents(true);
    SetRootComponent(Root);

    OrbBody = CreateDefaultSubobject<UStaticMeshComponent>("Orb");
    OrbBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    OrbBody->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AExperienceOrbBase::BeginPlay()
{
    Super::BeginPlay();

    
}

// Called every frame
void AExperienceOrbBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsCollected && CorrespondingCollector)
    {
        FVector CurrentLocation = GetActorLocation();

        CurrentLocation = FMath::VInterpConstantTo(
            CurrentLocation,
            CollectorLocation,
            DeltaTime,
            BaseSpeed
        );

        SetActorLocation(CurrentLocation);

        if ((CollectorLocation - CurrentLocation).Length() < 20.0f)
        {
            // should just hide and disable interactions and use object pooling for this
            UE_LOG(LogTemp, Warning, TEXT("Enemy Location: %f , %f"), GetActorLocation().X, GetActorLocation().Y);
            SetInUse(false);
        }
    }
}

void AExperienceOrbBase::UpdateColor(ERainbowColor color)
{
    OrbColor = color;

    if (MaterialList.Contains(OrbColor))
        OrbBody->SetMaterial(0, *(MaterialList.Find(OrbColor)));

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACollectorManager::StaticClass());

    if (FoundActor)
    {
        // Safely cast to the specific type to call its functions
        ACollectorManager* CollectorManager = Cast<ACollectorManager>(FoundActor);

        if (CollectorManager)
        {
            // this stores the result in AActor type because i only need it's location and not any class-specific functions
            CorrespondingCollector = CollectorManager->GetCollectorByColor(OrbColor);
            CollectorLocation = CorrespondingCollector->GetActorLocation();
        }
    }
}

void AExperienceOrbBase::SetCollected(bool isCollected)
{
    if (!bIsCollected)
    {
        bIsCollected = true;

        StartLocation = GetActorLocation();
    }
}

void AExperienceOrbBase::SetInUse(bool inUse)
{
    Super::SetInUse(inUse);

    if (!inUse)
        bIsCollected = false;
}

