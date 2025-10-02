#include "SkillBase.h"
#include "RPG_GameloftTestCharacter.h"
#include "WeaponBase.h"
#include "Animation/AnimInstance.h"


void USkillBase::ExecuteSkill_Implementation(ACharacter* PlayerCharacter)
{
    auto MyChar = Cast<ARPG_GameloftTestCharacter>(PlayerCharacter);
    if (!MyChar || !MyChar->CurrentWeapon) return;

    // call weapon attack
    MyChar->CurrentWeapon->PerformAttack(Damage, SkillType);
}
