#include "WeaponBase.h"
#include "RPG_GameloftTestCharacter.h"

AWeaponBase::AWeaponBase()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
}

void AWeaponBase::UseSkill(int32 Index)
{
    if (!WeaponSkills.IsValidIndex(Index) || !OwnerCharacter) return;

    const FWeaponSkillEntry& Entry = WeaponSkills[Index];
    PendingDamage = Entry.Damage;
    PendingSkillType = Entry.SkillType;
    PendingSkillIndex = Index;

    if (Entry.AttackMontage && OwnerCharacter)
    {
        UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(Entry.AttackMontage))
        {
            AnimInstance->Montage_Play(Entry.AttackMontage, 1.f);
        }
    }
}

void AWeaponBase::PerformAttack(float InDamage, ESkillType SkillType)
{

}

void AWeaponBase::EnableDamage() {}
void AWeaponBase::DisableDamage() {}

void AWeaponBase::Equip(ARPG_GameloftTestCharacter* NewOwner, FName SocketName)
{
    if (!NewOwner) return;
    OwnerCharacter = NewOwner;
    AttachToComponent(NewOwner->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        SocketName);
    SetOwner(NewOwner);
}

void AWeaponBase::Equip(ACharacter* NewOwner, FName SocketName)
{
    if (!NewOwner) return;

    OwnerCharacter = NewOwner;

    AttachToComponent(NewOwner->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        SocketName);

    SetOwner(NewOwner);
}



void AWeaponBase::Unequip()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    OwnerCharacter = nullptr;
}
