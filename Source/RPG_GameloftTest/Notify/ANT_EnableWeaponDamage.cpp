// Fill out your copyright notice in the Description page of Project Settings.

#include "ANT_EnableWeaponDamage.h"
#include "RPG_GameloftTest/WeaponBase.h"
#include <RPG_GameloftTest/RPG_GameloftTestCharacter.h>


void UANT_EnableWeaponDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    ACharacter* CharOwner = Cast<ACharacter>(Owner);
    if (!CharOwner) return;


    AWeaponBase* CurrentWeapon = nullptr;


    if (ARPG_GameloftTestCharacter* MyChar = Cast<ARPG_GameloftTestCharacter>(CharOwner))
    {
        CurrentWeapon = MyChar->CurrentWeapon;
    }

    else if (AEnemyBase* MyEnemy = Cast<AEnemyBase>(CharOwner))
    {
        CurrentWeapon = MyEnemy->CurrentWeapon;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->EnableDamage();
        CurrentWeapon->PerformAttack(
            CurrentWeapon->PendingDamage,
            CurrentWeapon->PendingSkillType
        );
    }
}


void UANT_EnableWeaponDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    ACharacter* CharOwner = Cast<ACharacter>(Owner);
    if (!CharOwner) return;


    if (ARPG_GameloftTestCharacter* MyChar = Cast<ARPG_GameloftTestCharacter>(CharOwner))
    {
        if (MyChar && MyChar->CurrentWeapon)
        {
            MyChar->CurrentWeapon->DisableDamage();
        }
    }
    else if (AEnemyBase* MyEnemy = Cast<AEnemyBase>(CharOwner))
    {
        if (MyEnemy && MyEnemy->CurrentWeapon)
        {
            MyEnemy->CurrentWeapon->DisableDamage();
        }
    }

}
