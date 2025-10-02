// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


AObstacleActor::AObstacleActor()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetCollisionProfileName(TEXT("BlockAll"));

    Health = 50.f;
}

void AObstacleActor::BeginPlay()
{
    Super::BeginPlay();
}

float AObstacleActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage, remaining health: %.1f"),
        *GetName(), DamageAmount, Health);

    if (Health <= 0.f)
    {
        // Spawn particle explosion
        if (ExplodeEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                ExplodeEffect,
                GetActorLocation(),
                GetActorRotation(),
                true
            );
        }
        if (ExploseSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, ExploseSound, GetActorLocation(), 0.2f);
        }
        Destroy();
    }

    return DamageApplied;
}