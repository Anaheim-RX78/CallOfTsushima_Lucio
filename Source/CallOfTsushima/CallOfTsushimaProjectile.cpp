// Copyright Epic Games, Inc. All Rights Reserved.

#include "CallOfTsushimaProjectile.h"

#include "CallOfTsushimaCharacter.h"
#include "PaintableSurface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

void ACallOfTsushimaProjectile::DestroyActor(AActor* ActorToDestroy)
{
	ActorToDestroy->Destroy();
}

void ACallOfTsushimaProjectile::SpawnVFX(const FHitResult& Hit)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
	
		// Spawn the projectile at the muzzle
		World->SpawnActor<AActor>(ActorToSpawn, Hit.Location, Hit.Normal.Rotation(), ActorSpawnParams);
	}
}

void ACallOfTsushimaProjectile::BroadcastDestroy_Implementation(AActor* ActorToDestroy)
{
	DestroyActor(ActorToDestroy);
}

void ACallOfTsushimaProjectile::BroadcastVFX_Implementation(const FHitResult& Hit)
{
	SpawnVFX(Hit);
}

ACallOfTsushimaProjectile::ACallOfTsushimaProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ACallOfTsushimaProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ACallOfTsushimaProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACallOfTsushimaProjectile, ColorTag);
}

void ACallOfTsushimaProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("My tag is: %s"), *ColorTag.ToString()));
	if (Cast<APaintableSurface>(OtherActor))
	{
		Destroy();
		return;
	}

	if (ACallOfTsushimaCharacter* TPChar = Cast<ACallOfTsushimaCharacter>(OtherActor))
	{
		TPChar->OnHitByProjectile.Broadcast();
		Destroy();
	}
	

}
