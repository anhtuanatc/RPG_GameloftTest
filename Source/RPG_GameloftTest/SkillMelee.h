#pragma once
#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SkillMelee.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class USkillMelee : public USkillBase
{
    GENERATED_BODY()

public:
    virtual void ExecuteSkill_Implementation(ACharacter* PlayerCharacter);
};
