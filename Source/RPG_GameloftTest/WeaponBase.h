#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.h"
#include "Animation/AnimMontage.h"
#include "Enemies/EnemyBase.h"
#include "WeaponBase.generated.h"


// Struct: 1 skill gắn với 1 montage
USTRUCT(BlueprintType)

struct FWeaponSkillEntry
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* AttackMontage;

    FWeaponSkillEntry()
        : SkillType(ESkillType::Melee)
        , Damage(10.f)
        , AttackMontage(nullptr)
    {
    }
};

UCLASS(Abstract, Blueprintable)
class RPG_GAMELOFTTEST_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    //list skill + montage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<FWeaponSkillEntry> WeaponSkills;

  
    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    ACharacter* OwnerCharacter;

    float PendingDamage;
    ESkillType PendingSkillType;
    int32 PendingSkillIndex;

    // Call skill index (Skill1 = index 0)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void UseSkill(int32 Index);

    virtual void PerformAttack(float InDamage, ESkillType SkillType);


    virtual void EnableDamage();
    virtual void DisableDamage();

    // Equip / Unequip
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Equip(class ARPG_GameloftTestCharacter* NewOwner, FName SocketName = "WeaponSocket");
    virtual void Equip(ACharacter* NewOwner, FName SocketName = "WeaponSocket");
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Unequip();
};
