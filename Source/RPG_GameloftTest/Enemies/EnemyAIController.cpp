#include "EnemyAIController.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include <RPG_GameloftTest/RPG_GameloftTestCharacter.h>
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    ControlledEnemy = nullptr;
    PlayerChar = nullptr;
    CurrentState = EAIState::Idle;

    AttackCooldown = 1.5f;
    TimeSinceLastAttack = AttackCooldown;

    MeleeAttackRange = 150.f;
    MeleeAcceptanceRadius = 50.f;

    RangedMaxRange = 500.f;
    RangedAcceptanceRadius = 100.f;


}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ControlledEnemy = Cast<AEnemyBase>(InPawn);
    PlayerChar = Cast<ARPG_GameloftTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!ControlledEnemy || ControlledEnemy->IsDead()) return;

    if (!PlayerChar)
    {
        PlayerChar = Cast<ARPG_GameloftTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (!PlayerChar) return;
    }
    // if player die -> stop
    if (PlayerChar->bIsDead)
    {
        StopMovement();
        return;
    }
    TimeSinceLastAttack += DeltaSeconds;
    const float Distance = FVector::Dist(PlayerChar->GetActorLocation(), ControlledEnemy->GetActorLocation());

    switch (ControlledEnemy->EnemyType)
    {
    case EEnemyType::Melee:  HandleMelee(DeltaSeconds, Distance);  break;
    case EEnemyType::Ranged: HandleRanged(DeltaSeconds, Distance); break;
    default: break;
    }
}

void AEnemyAIController::HandleMelee(float DeltaSeconds, float Distance)
{
    if (Distance > MeleeAttackRange)
    {
        if (CurrentState != EAIState::Chasing)
        {
            MoveToActor(PlayerChar, MeleeAcceptanceRadius);
            CurrentState = EAIState::Chasing;
        }
    }
    else
    {
        CurrentState = EAIState::Attacking;
        StopMovement();
        AttackPlayer(DeltaSeconds);
    }
}

void AEnemyAIController::HandleRanged(float DeltaSeconds, float Distance)
{
    if (Distance > RangedMaxRange)
    {
        if (CurrentState != EAIState::Chasing)
        {
            MoveToActor(PlayerChar, RangedAcceptanceRadius);
            CurrentState = EAIState::Chasing;
        }
    }
    else
    {
        CurrentState = EAIState::Attacking;
        StopMovement();
        AttackPlayer(DeltaSeconds);
    }
}

void AEnemyAIController::AttackPlayer(float DeltaSeconds)
{
    if (!ControlledEnemy || !PlayerChar) return;

    if (ControlledEnemy->EnemyType == EEnemyType::Melee)
    {
        // Melee: instant rotate + attack on cooldown
        RotateTowardsPlayerInstant();
        if (TimeSinceLastAttack >= AttackCooldown)
        {
            ControlledEnemy->Attack(0);
            TimeSinceLastAttack = 0.f;
        }
    }
    else
    {
        // Ranged: smooth rotate, only fire when facing player
        RotateTowardsPlayerSmooth(DeltaSeconds);

        FVector ToPlayer = PlayerChar->GetActorLocation() - ControlledEnemy->GetActorLocation();
        ToPlayer.Z = 0.f;
        ToPlayer.Normalize();

        float Dot = FVector::DotProduct(ControlledEnemy->GetActorForwardVector(), ToPlayer);
        const float FacingThreshold = 0.95f;

        if (Dot >= FacingThreshold && TimeSinceLastAttack >= AttackCooldown)
        {
            ControlledEnemy->Attack(0);
            TimeSinceLastAttack = 0.f;
        }
    }
}

void AEnemyAIController::RotateTowardsPlayerInstant()
{
    if (!ControlledEnemy || !PlayerChar) return;

    FVector Dir = PlayerChar->GetActorLocation() - ControlledEnemy->GetActorLocation();
    Dir.Z = 0.f;
    if (Dir.IsNearlyZero()) return;

    ControlledEnemy->SetActorRotation(Dir.Rotation());
}

void AEnemyAIController::RotateTowardsPlayerSmooth(float DeltaSeconds)
{
    if (!ControlledEnemy || !PlayerChar) return;

    FVector Dir = PlayerChar->GetActorLocation() - ControlledEnemy->GetActorLocation();
    Dir.Z = 0.f;
    if (Dir.IsNearlyZero()) return;

    FRotator Target = Dir.Rotation();
    float RotateSpeed = 8.f;
    FRotator NewRot = FMath::RInterpTo(ControlledEnemy->GetActorRotation(), Target, DeltaSeconds, RotateSpeed);
    ControlledEnemy->SetActorRotation(NewRot);
}
void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!ControlledEnemy || !PlayerChar) return;

    float Distance = FVector::Dist(ControlledEnemy->GetActorLocation(), PlayerChar->GetActorLocation());

    if (Result.Code == EPathFollowingResult::Success)
    {
        // if move success -> check distance for attack or idle
        if (Distance >= IdealMinRange && Distance <= IdealMaxRange)
        {
            CurrentState = EAIState::Attacking;
        }
        else
        {
            CurrentState = EAIState::Idle;
        }
    }
    else
    {
        // if move false
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys && PlayerChar)
        {
            FNavLocation ProjectedLoc;
            if (NavSys->ProjectPointToNavigation(
                PlayerChar->GetActorLocation(),
                ProjectedLoc,
                FVector(200.f, 200.f, 500.f))) // find NavMesh arround player
            {
                // run to near player
                MoveToLocation(ProjectedLoc.Location, 50.f);
                CurrentState = EAIState::Chasing;
                return;
            }
        }

        // If still cannot find NavMesh
        if (ControlledEnemy->EnemyType == EEnemyType::Melee && Distance <= MeleeAttackRange)
        {
            CurrentState = EAIState::Attacking;
        }
        else if (ControlledEnemy->EnemyType == EEnemyType::Ranged && Distance <= RangedMaxRange)
        {
            CurrentState = EAIState::Attacking;
        }
        else
        {
            CurrentState = EAIState::Idle;
        }
    }
}
