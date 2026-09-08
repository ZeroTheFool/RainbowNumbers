// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyShapeBase.h"
#include "PlayerShape.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossEnemyShapeBase::ABossEnemyShapeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USphereComponent>("Root");
    Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Root->SetCollisionProfileName("OverlapAll"); // needs to ignore everything but player
    Root->SetGenerateOverlapEvents(true);
    Root->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemyShapeBase::OnOverlapBegin);
    SetRootComponent(Root);

    EnemyBody = CreateDefaultSubobject<UStaticMeshComponent>("Enemy");
    EnemyBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyBody->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABossEnemyShapeBase::BeginPlay()
{
	Super::BeginPlay();
	
    TemporaryTargetLocation = TargetLocation;
}

// Called every frame
void ABossEnemyShapeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();

    CurrentLocation = FMath::VInterpConstantTo(
        CurrentLocation,
        TemporaryTargetLocation,
        DeltaTime,
        BaseSpeed
    );

    SetActorLocation(CurrentLocation);

    if ((TemporaryTargetLocation - CurrentLocation).Length() < 20.0f)
    {
        TemporaryTargetLocation = TargetLocation;
        BaseSpeed += 20.0f;
    }

    if ((TargetLocation - CurrentLocation).Length() < 20.0f)
    {
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShape::StaticClass());
        if (FoundActor)
        {
            FoundActor->Destroy();
        }
    }

}

void ABossEnemyShapeBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (OtherActor)
    {
        APlayerShape* Player = Cast<APlayerShape>(OtherActor);
        if (Player)
        {
            // this is to take and deal damage
            Player->ReceiveDamage(Damage);

            // get direction away from player??
            FVector DirectionToPlayer = GetActorLocation() - Player->GetActorLocation();
            DirectionToPlayer.Normalize();
            TemporaryTargetLocation = GetActorLocation() + (DirectionToPlayer * FVector(200.f, 200.f, 0.0f));

            DamageValueStored = Player->GetDamageValue();

            GetWorld()->GetTimerManager().ClearTimer(ContinuousDamage_Timer);

            GetWorld()->GetTimerManager().SetTimer(ContinuousDamage_Timer, this, &ABossEnemyShapeBase::OnContinuousDamage, DamageDelay, true);
        }
    }
}

void ABossEnemyShapeBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        APlayerShape* Player = Cast<APlayerShape>(OtherActor);
        if (Player)
        {
            GetWorld()->GetTimerManager().ClearTimer(ContinuousDamage_Timer);
            DamageValueStored = 0.0f;
        }
    }
}

void ABossEnemyShapeBase::ReceiveDamage(float inDamage)
{
    Health -= inDamage;

    if (Health <= 0.0f)
    {
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShape::StaticClass());
        if (FoundActor)
        {
            FoundActor->Destroy();
        }
    }
}

void ABossEnemyShapeBase::OnContinuousDamage()
{
    ReceiveDamage(DamageValueStored);
}