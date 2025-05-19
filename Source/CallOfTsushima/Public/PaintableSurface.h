// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaintableSurface.generated.h"

UENUM()
enum class EPaintColor : uint8
{
	None,
	Red,
	Blue
};

UCLASS()
class CALLOFTSUSHIMA_API APaintableSurface : public AActor
{
	GENERATED_BODY()
	
public:
	APaintableSurface();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PaintCellAtWorldLocation(const FHitResult& Hit, FLinearColor Color);
	void UpdateColorStats();

public:
	UPROPERTY(VisibleAnywhere, Category = "PaintableSurface")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "PaintableSurface")
	float GridCellSize = 20.f;

	UPROPERTY(EditAnywhere, Category = "PaintableSurface")
	UMaterialInterface* PaintProjectionMaterialBase;

	UPROPERTY(EditAnywhere, Category = "PaintableSurface")
	UMaterialInterface* DecalMaterialBase;

	UPROPERTY()
	TMap<FIntPoint, UDecalComponent*> TemporaryDecals;

	UPROPERTY(VisibleAnywhere)
	float BakeTimer = 0;

	UPROPERTY(EditAnywhere, Category = "PaintableSurface")
	float BakePeriodInSeconds = 2;
	

private:
	TMap<FIntPoint, FLinearColor> PaintedGrid;
};