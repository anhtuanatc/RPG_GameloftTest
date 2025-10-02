#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class RPG_GAMELOFTTEST_API AMeleeWeapon : public AWeaponBase
{
    GENERATED_BODY()

public:
    AMeleeWeapon();

protected:
    // Box for detect hit
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    class UBoxComponent* DamageCollision;

    // Damage
    float CurrentDamage;

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:
    // Override from WeaponBase
    virtual void PerformAttack(float InDamage, ESkillType SkillType) override;
    virtual void EnableDamage() override;
    virtual void DisableDamage() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    USoundBase* AttackSound;


};
