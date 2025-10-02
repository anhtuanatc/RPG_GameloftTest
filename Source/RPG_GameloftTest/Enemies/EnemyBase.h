#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnemyBase.generated.h"
class AWeaponBase;


UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    Melee UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged")
};


UCLASS()
class RPG_GAMELOFTTEST_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    EEnemyType EnemyType;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
    float CurrentHealth;



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    TSubclassOf<AWeaponBase> WeaponClass;




public:
    virtual void BeginPlay() override;


    UPROPERTY(BlueprintReadOnly, Category = "Enemy")
    AWeaponBase* CurrentWeapon;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator, AActor* DamageCauser) override;

    void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* HitReactMontage;
    UAnimMontage* DeathMontage;

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void Attack(int32 SkillIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    bool IsDead() const { return CurrentHealth <= 0.f; }

    bool bIsDead = false;
    void EnterRagdoll();

    float GetAttackRange() const;
};
