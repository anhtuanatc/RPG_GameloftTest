#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleActor.generated.h"

UCLASS()
class RPG_GAMELOFTTEST_API AObstacleActor : public AActor
{
    GENERATED_BODY()

public:
    AObstacleActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
    float Health;

    UPROPERTY(EditAnywhere, Category = "Effects")
    class UParticleSystem* ExplodeEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* ExploseSound;

public:
    virtual void BeginPlay() override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator, AActor* DamageCauser) override;
};
