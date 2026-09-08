// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceCollectorBase.h"
#include "ExperienceOrbBase.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AExperienceCollectorBase::AExperienceCollectorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
    Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Root->SetCollisionProfileName("OverlapAll");
    Root->SetGenerateOverlapEvents(true);
    Root->OnComponentBeginOverlap.AddDynamic(this, &AExperienceCollectorBase::OnOverlapBegin);

    CollectorBody = CreateDefaultSubobject<UStaticMeshComponent>("Collector");
    CollectorBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollectorBody->SetupAttachment(Root);

    OrbCountText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CustomTextRender"));

    OrbCountText->SetupAttachment(RootComponent);

    OrbCountText->SetText(FText::AsNumber(OrbCount));
    OrbCountText->SetWorldSize(75.0f); // Height of the text font in world units

    OrbCountText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    OrbCountText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
    OrbCountText->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AExperienceCollectorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExperienceCollectorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExperienceCollectorBase::SetOrbCount(int32 count)
{
    OrbCount = count;
    OrbCountText->SetText(FText::AsNumber(OrbCount));
}

void AExperienceCollectorBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (OtherActor)
    {
        AExperienceOrbBase* Orb = Cast<AExperienceOrbBase>(OtherActor);
        if (Orb)
        {
            if (CollectorColor == Orb->CheckColor())
            {
                OrbCount++;
                OrbCountText->SetText(FText::AsNumber(OrbCount));
            }
        }
    }
}