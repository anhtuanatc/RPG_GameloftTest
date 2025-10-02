
#include "EnemyBase.h"
#include "RPG_GameloftTest/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include <RPG_GameloftTest/RPG_GameloftTestCharacter.h>


AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
    CurrentWeapon = nullptr;
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    if (WeaponClass)
    {
        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = this;

        CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, Params);
        if (CurrentWeapon)
        {
            // Attach socket
            CurrentWeapon->Equip(this, TEXT("WeaponSocket"));
        }
    }
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
 
    if (AppliedDamage <= 0.f || bIsDead) return 0.f;

    CurrentHealth -= AppliedDamage;

    if (CurrentHealth <= 0.f)
    {
        bIsDead = true;
        // 1) Stop AI and movement
        if (AController* Ctrl = GetController())
        {
            Ctrl->UnPossess();
        }
        if (UCapsuleComponent* Cap = GetCapsuleComponent())
        {
            Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        if (GetCharacterMovement())
        {
            GetCharacterMovement()->DisableMovement();
        }

        // 2) Unequip weapon
        if (CurrentWeapon)
        {
            CurrentWeapon->Unequip(); 
            CurrentWeapon->Destroy();
            CurrentWeapon = nullptr;
        }

        // 3) Play death montage
        if (DeathMontage)
        {
            if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
            {
                // bind delegate
                FOnMontageEnded MontageEndDelegate;
                MontageEndDelegate.BindUObject(this, &AEnemyBase::OnDeathMontageEnded);
                Anim->Montage_Play(DeathMontage, 1.f);
                Anim->Montage_SetEndDelegate(MontageEndDelegate, DeathMontage);
            }
            else
            {
                EnterRagdoll();
            }
        }
        else
        {
            EnterRagdoll();
        }

        // cleanup
        SetLifeSpan(8.f);
    }
    else
    {
        // Hit react
        if (HitReactMontage)
        {
            if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
            {
                if (!Anim->Montage_IsPlaying(HitReactMontage))
                {
                    Anim->Montage_Play(HitReactMontage, 1.f);
                }
            }
        }
    }

    return AppliedDamage;
}

void AEnemyBase::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    EnterRagdoll();
}

void AEnemyBase::EnterRagdoll()
{
    // turn off anim BP control
    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp) return;

    //detach capsule collision
    if (UCapsuleComponent* Cap = GetCapsuleComponent())
    {
        Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    MeshComp->SetAllBodiesSimulatePhysics(true);
    MeshComp->SetSimulatePhysics(true);
    MeshComp->WakeAllRigidBodies();
    MeshComp->bBlendPhysics = true;

    // change collision profile
    MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));

    // off tick anim instance
    if (MeshComp->GetAnimInstance())
    {
        MeshComp->GetAnimInstance()->StopAllMontages(0.f);
    }
}
float AEnemyBase::GetAttackRange() const
{
    switch (EnemyType)
    {
    case EEnemyType::Melee:  return 150.f;
    case EEnemyType::Ranged: return 500.f; 
    default: return 300.f;
    }
}


void AEnemyBase::Attack(int32 SkillIndex)
{
    if (CurrentWeapon && !IsDead())
    {
        CurrentWeapon->UseSkill(SkillIndex);
    }
}