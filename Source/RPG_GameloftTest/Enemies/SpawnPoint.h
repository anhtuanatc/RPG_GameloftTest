
#pragma once
#include "EnemyBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"


class AEnemyBase;

UCLASS()
class RPG_GAMELOFTTEST_API ASpawnPoint : public AActor
{
    GENERATED_BODY()

public:
    ASpawnPoint();

protected:
    virtual void BeginPlay() override;

    // Timer handle để spawn theo interval
    FTimerHandle SpawnTimerHandle;

    // Danh sách enemy đang sống
    UPROPERTY()
    TArray<AEnemyBase*> ActiveEnemies;

    // Spawn 1 enemy
    void SpawnEnemy();

    // Callback khi enemy chết
    UFUNCTION()
    void OnEnemyDied(AActor* DeadEnemy);

public:
    // Enemy Melee class
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AEnemyBase> MeleeEnemyClass;

    // Enemy Ranged class
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<AEnemyBase> RangedEnemyClass;

    // Max enemies
    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MaxEnemies = 5;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float SpawnInterval = 5.f;

    // Ratio spawn (3 melee : 1 ranged)
    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MeleeToRangedRatio = 3;
};
