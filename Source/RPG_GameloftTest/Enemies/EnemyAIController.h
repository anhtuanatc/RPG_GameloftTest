#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Chasing,
    Retreating,
    Attacking
};

class AEnemyBase;
class ARPG_GameloftTestCharacter;

UCLASS()
class RPG_GAMELOFTTEST_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


private:
    // refs
    UPROPERTY()
    AEnemyBase* ControlledEnemy;

    UPROPERTY()
    ARPG_GameloftTestCharacter* PlayerChar;

    // state
    EAIState CurrentState;

    // attack / timing
    float AttackCooldown;
    float TimeSinceLastAttack;

    // ranges & acceptance radii (tweak values here)
    float MeleeAttackRange;
    float MeleeAcceptanceRadius;

    float RangedMinRange;
    float RangedMaxRange;
    float RangedRetreatDistance;

   
    float RangedAcceptanceRadius = 80.f; // acceptance when MoveToActor/MoveToLocation

    const float IdealMinRange = 200.f; 
    const float IdealMaxRange = 300.f; 


    // handlers
    void HandleMelee(float DeltaSeconds, float Distance);
    void HandleRanged(float DeltaSeconds, float Distance);

    // attack helpers

    void AttackPlayer(float DeltaSeconds);

    // rotation helpers
    void RotateTowardsPlayerInstant();
    void RotateTowardsPlayerSmooth(float DeltaSeconds);

};
