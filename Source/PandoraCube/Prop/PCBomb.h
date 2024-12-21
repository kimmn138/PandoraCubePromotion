// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "PCBomb.generated.h"

/**
 * 
 */
UCLASS()
class PANDORACUBE_API APCBomb : public AFieldSystemActor
{
	GENERATED_BODY()
	
public:
	APCBomb();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UFieldSystemComponent> FieldSystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Field")
	TObjectPtr<class URadialFalloff> RadialFalloffNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<class UParticleSystem> ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<class USoundBase> ExplosionSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> GasTankMesh;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TObjectPtr<class APCSpawnManager> SpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", meta = (ClampMin = "0"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb")
	float ExplosionDamage = 400.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb")
	float ExplosionRadius = 500.0f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Explode();
};
