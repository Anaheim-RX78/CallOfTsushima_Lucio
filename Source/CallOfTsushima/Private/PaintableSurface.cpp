// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintableSurface.h"

#include "CanvasItem.h"
#include "CallOfTsushima/CallOfTsushimaProjectile.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APaintableSurface::APaintableSurface()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionProfileName("BlockAll");
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->OnComponentHit.AddDynamic(this, &APaintableSurface::OnHit);
}

void APaintableSurface::BeginPlay()
{
    Super::BeginPlay();
    
}

void APaintableSurface::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (ACallOfTsushimaProjectile* Proj = Cast<ACallOfTsushimaProjectile>(OtherActor))
    {
        FLinearColor Color = Proj->ColorTag == FName("Red") ? FLinearColor::Red : FLinearColor::Blue;
        PaintCellAtWorldLocation(Hit, Color);
    }
}

void APaintableSurface::PaintCellAtWorldLocation(const FHitResult& Hit, FLinearColor Color)
{
    // FVector Local = GetActorTransform().InverseTransformPosition(Hit.ImpactPoint);
    FVector SnappedLocation;
    SnappedLocation.X = FMath::FloorToFloat(Hit.ImpactPoint.X / GridCellSize) * GridCellSize + GridCellSize * 0.5f;
    SnappedLocation.Y = FMath::FloorToFloat(Hit.ImpactPoint.Y / GridCellSize) * GridCellSize + GridCellSize * 0.5f;
    SnappedLocation.Z = Hit.ImpactPoint.Z;

    FIntPoint Coords;
    Coords.X = FMath::FloorToInt(SnappedLocation.X / GridCellSize);
    Coords.Y = FMath::FloorToInt(SnappedLocation.Y / GridCellSize);

    UMaterialInstanceDynamic* DecalMID = UMaterialInstanceDynamic::Create(DecalMaterialBase, this);
    DecalMID->SetVectorParameterValue("PaintColor", Color);
    
    if (!PaintedGrid.Contains(Coords) || PaintedGrid[Coords] != Color)
    {
        if (!PaintedGrid.Contains(Coords))
        {
            PaintedGrid.Add(Coords, Color);
        }
        else
        {
            PaintedGrid[Coords] = Color;
            TemporaryDecals[Coords]->DestroyComponent();
        }
        UpdateColorStats();

        FVector DecalSize(1, GridCellSize * 0.5f, GridCellSize * 0.5f); // Box size (X = depth, Y/Z = width/height)

        UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
            GetWorld(),
            DecalMaterialBase,
            DecalSize,
            SnappedLocation,
            Hit.ImpactNormal.Rotation(), // Align with surface
            0
        );

        Decal->SetDecalMaterial(DecalMID);
        TemporaryDecals.Add(Coords, Decal);
    }
}

void APaintableSurface::UpdateColorStats()
{
    int32 Total = PaintedGrid.Num();
    int32 Red = 0;
    int32 Blue = 0;

    for (const auto& Pair : PaintedGrid)
    {
        if (Pair.Value == FLinearColor::Red) Red++;
        else if (Pair.Value == FLinearColor::Blue) Blue++;
    }

    float RedPct = Total > 0 ? (100.f * Red) / Total : 0.f;
    float BluePct = Total > 0 ? (100.f * Blue) / Total : 0.f;

    UE_LOG(LogTemp, Log, TEXT("Red: %.1f%% | Blue: %.1f%% (%d total cells)"), RedPct, BluePct, Total);
}

// Called every frame
void APaintableSurface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // if (TemporaryDecals.Num() == 0) return;
    return;

    BakeTimer += DeltaTime;

    if (BakeTimer > BakePeriodInSeconds)
    {
        BakeTimer = 0;
    }
}

