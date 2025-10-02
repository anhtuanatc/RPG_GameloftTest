#include "StaffWeapon.h"
#include "RPG_GameloftTestCharacter.h"                // để lấy OwnerCharacter
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "FireballProjectile.h"

AStaffWeapon::AStaffWeapon()
{
    MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
    MuzzlePoint->SetupAttachment(WeaponMesh);
}

void AStaffWeapon::PerformAttack(float InDamage, ESkillType SkillType)
{
    if (SkillType == ESkillType::Magic && ProjectileClass && OwnerCharacter)
    {
        FVector SpawnLoc = MuzzlePoint->GetComponentLocation();

        //Shoot to forward owner
        FRotator SpawnRot = OwnerCharacter->GetActorForwardVector().Rotation();

        FActorSpawnParameters Params;
        Params.Owner = OwnerCharacter;
        Params.Instigator = OwnerCharacter;

        AFireballProjectile* Projectile = GetWorld()->SpawnActor<AFireballProjectile>(ProjectileClass, SpawnLoc, SpawnRot, Params);
        if (Projectile)
        {
            Projectile->Damage = InDamage;
        }
        if (AttackSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(),0.2f);
        }
    }
}

