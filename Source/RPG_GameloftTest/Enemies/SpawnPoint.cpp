
#include "SpawnPoint.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASpawnPoint::ASpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASpawnPoint::BeginPlay()
{
    Super::BeginPlay();

    // loop spawn
    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &ASpawnPoint::SpawnEnemy,
        SpawnInterval,
        true
    );
}

void ASpawnPoint::SpawnEnemy()
{
    if (ActiveEnemies.Num() >= MaxEnemies) return;
    if (!MeleeEnemyClass && !RangedEnemyClass) return;

    bool bSpawnMelee = true;
    if (MeleeToRangedRatio > 0)
    {
        int32 Roll = FMath::RandRange(1, MeleeToRangedRatio + 1);
        bSpawnMelee = (Roll <= MeleeToRangedRatio);
    }

    TSubclassOf<AEnemyBase> EnemyClass = bSpawnMelee ? MeleeEnemyClass : RangedEnemyClass;
    if (!EnemyClass) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(
        EnemyClass,
        GetActorLocation(),
        GetActorRotation(),
        SpawnParams
    );

    if (NewEnemy)
    {
        ActiveEnemies.Add(NewEnemy);
        NewEnemy->OnDestroyed.AddDynamic(this, &ASpawnPoint::OnEnemyDied);

        // --- ENSURE AI CONTROLLER IS SPAWNED / POSSESSED ---
        if (!NewEnemy->GetController())
        {
            NewEnemy->SpawnDefaultController();
        }

    }
}

void ASpawnPoint::OnEnemyDied(AActor* DeadEnemy)
{
    ActiveEnemies.Remove(Cast<AEnemyBase>(DeadEnemy));
}
