// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CallOfTsushimaProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class ACallOfTsushimaProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void DestroyActor(AActor* ActorToDestroy);

	UFUNCTION(Server, Reliable)
	void BroadcastDestroy(AActor* ActorToDestroy);

	UFUNCTION()
	void SpawnVFX(const FHitResult& Hit);

	UFUNCTION(NetMulticast, Unreliable)
	void BroadcastVFX(const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<AActor> ActorToSpawn;

public:
	ACallOfTsushimaProjectile();


	UPROPERTY(EditAnywhere, Replicated)
	FName ColorTag;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

