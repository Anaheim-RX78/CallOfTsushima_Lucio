// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostGameState.h"
#include "Kismet/GameplayStatics.h"

void AGhostGameState::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaintableSurface::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (APaintableSurface* PaintableSurface = Cast<APaintableSurface>(Actor))
		{
			Paintables.Add(PaintableSurface);
		}
	}

	// Request->SetURL("https://meowfacts.herokuapp.com/");
}

TMap<EPaintColor, int> AGhostGameState::GetAllColors()
{
	TArray<TMap<EPaintColor, int>> PaintResults = {};
	for (APaintableSurface* PaintableSurface : Paintables)
	{
		PaintResults.Add(PaintableSurface->GetColorStats());
	}

	TMap<EPaintColor, int> FinalResult = {};
	for (const TMap<EPaintColor, int>& SurfaceResult : PaintResults)
	{
		for (const TPair<EPaintColor, int>& Pair : SurfaceResult)
		{
			FinalResult.FindOrAdd(Pair.Key) += Pair.Value;
		}
	}

	return FinalResult;
}