// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "EnemyShapeBase.h"
#include "BossEnemyShapeBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
    Root->SetSphereRadius(16.0f);
    Root->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Root->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
    Root->SetCollisionResponseToAllChannels(ECR_Block);
    Root->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

    // create the projectile movement component. No need to attach it because it's not a Scene Component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AProjectile::SetInUse(bool inUse)
{
    Super::SetInUse(inUse);

    if (bInUse)
    {
        ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;

    }
    else
    {
        ProjectileMovement->Velocity = FVector(0.0f);
    }

}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (AEnemyShapeBase* Enemy = Cast<AEnemyShapeBase>(OtherActor))
    {
        Enemy->ReceiveDamage(BaseDamage);

        SetInUse(false);
    }
    else if (ABossEnemyShapeBase* Boss = Cast<ABossEnemyShapeBase>(OtherActor))
    {
        Boss->ReceiveDamage(BaseDamage);
    }
}
