#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "StaffWeapon.generated.h"

UCLASS()
class RPG_GAMELOFTTEST_API AStaffWeapon : public AWeaponBase
{
    GENERATED_BODY()

public:
    AStaffWeapon();
    // Class projectile sẽ spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    USoundBase* AttackSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USceneComponent* MuzzlePoint;

    virtual void PerformAttack(float InDamage, ESkillType SkillType) override;
protected:


};
