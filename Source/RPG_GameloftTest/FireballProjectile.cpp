#include "FireballProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


#include <RPG_GameloftTest/RPG_GameloftTestCharacter.h>

AFireballProjectile::AFireballProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Mesh root
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Collision sphere
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetupAttachment(Mesh);
    Collision->InitSphereRadius(16.f);
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Collision->SetCollisionObjectType(ECC_WorldDynamic);
    Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
    Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    Collision->SetGenerateOverlapEvents(true);

    // Projectile movement
    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->InitialSpeed = 2000.f;
    Movement->MaxSpeed = 2000.f;
    Movement->bRotationFollowsVelocity = true;

    Damage = 30.f;
    bHasHit = false;
}

void AFireballProjectile::BeginPlay()
{
    Super::BeginPlay();

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AFireballProjectile::OnOverlapBegin);
    // ignore instigator/owner
    if (GetInstigator())
    {
        Collision->IgnoreActorWhenMoving(GetInstigator(), true);
    }
    if (GetOwner())
    {
        Collision->IgnoreActorWhenMoving(GetOwner(), true);
    }

    UE_LOG(LogTemp, Warning, TEXT("Projectile Collision Enabled=%d, OverlapEvents=%d"),
        Collision->GetCollisionEnabled(), Collision->GetGenerateOverlapEvents());

}

UFUNCTION()
void AFireballProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bHasHit) return;
    if (!OtherActor || OtherActor == this) return;

    if (OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

    ACharacter* VictimChar = Cast<ACharacter>(OtherActor);
    APawn* InstigatorPawn = Cast<APawn>(GetInstigator());

    if (VictimChar && InstigatorPawn)
    {
        if (InstigatorPawn->IsPlayerControlled() && !VictimChar->IsPlayerControlled())
        {
            UGameplayStatics::ApplyDamage(VictimChar, Damage, InstigatorPawn->GetController(), this, nullptr);
        }
        else if (!InstigatorPawn->IsPlayerControlled() && VictimChar->IsPlayerControlled())
        {
            UGameplayStatics::ApplyDamage(VictimChar, Damage, InstigatorPawn->GetController(), this, nullptr);
        }
    }
    else
    {
        if (InstigatorPawn && InstigatorPawn->IsPlayerControlled())
        {
            UGameplayStatics::ApplyDamage(OtherActor, Damage, InstigatorPawn->GetController(), this, nullptr);
        }
    }

    // --- Spawn FX ---
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = FRotator::ZeroRotator;

    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ImpactEffect,
            GetActorLocation(),
            GetActorRotation()
        );
    }


    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, SpawnLocation);
    }

    bHasHit = true;
    Destroy();
}
