#include "MeleeWeapon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "RPG_GameloftTestCharacter.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{
    DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
    DamageCollision->SetupAttachment(WeaponMesh); // WeaponMesh from WeaponBase (StaticMesh)
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    DamageCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);


    CurrentDamage = 0.f;
    DisableDamage();

}

void AMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();

    DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
}

void AMeleeWeapon::PerformAttack(float InDamage, ESkillType SkillType)
{
    if (SkillType == ESkillType::Melee)
    {
        // get damage
        CurrentDamage = InDamage;
        if (AttackSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(),0.2f);
        }
    }
}

void AMeleeWeapon::EnableDamage()
{
    if (DamageCollision)
    {
        DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

void AMeleeWeapon::DisableDamage()
{
    if (DamageCollision)
    {
        DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AMeleeWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    AActor* Victim = OtherActor;
    if (!Victim || Victim == OwnerCharacter) return;

    if (ACharacter* VictimChar = Cast<ACharacter>(Victim))
    {
        // --- Combat logic Player vs Enemy ---
        if (OwnerCharacter && OwnerCharacter->IsPlayerControlled() && !VictimChar->IsPlayerControlled())
        {
            // Player damage Enemy
            UGameplayStatics::ApplyDamage(VictimChar, CurrentDamage, OwnerCharacter->GetController(), this, nullptr);
        }
        else if (OwnerCharacter && !OwnerCharacter->IsPlayerControlled() && VictimChar->IsPlayerControlled())
        {
            // Enemy damage Player
            UGameplayStatics::ApplyDamage(VictimChar, CurrentDamage, OwnerCharacter->GetController(), this, nullptr);
        }
    }
    else
    {
        // --- Player damage obstacle  ---
        if (OwnerCharacter && OwnerCharacter->IsPlayerControlled())
        {
            UGameplayStatics::ApplyDamage(Victim, CurrentDamage, OwnerCharacter->GetController(), this, nullptr);
        }
    }

}
