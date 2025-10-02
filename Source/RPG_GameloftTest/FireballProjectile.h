// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h" 

class UNiagaraSystem;


UCLASS()
class RPG_GAMELOFTTEST_API AFireballProjectile : public AActor
{
    GENERATED_BODY()

public:
    AFireballProjectile();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float Damage;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* ImpactEffect;


    UPROPERTY(EditDefaultsOnly, Category = "SFX")
    USoundBase* ExplosionSound;


protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UProjectileMovementComponent* Movement;


    bool bHasHit;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
