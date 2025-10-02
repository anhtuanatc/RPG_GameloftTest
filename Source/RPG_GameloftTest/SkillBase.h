#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Melee,
    Magic,
    Ranged
};

UCLASS(Blueprintable, Abstract)
class RPG_GAMELOFTTEST_API USkillBase : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
    void ExecuteSkill(ACharacter* PlayerCharacter);
    virtual void ExecuteSkill_Implementation(ACharacter* PlayerCharacter);
};
