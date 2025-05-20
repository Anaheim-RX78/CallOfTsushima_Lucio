// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AGhostGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameTimer = TimerInSeconds;
}

void AGhostGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTimer += DeltaTime;

	if (CurrentTimer >= 1)
	{
		CurrentTimer -= 1;
		CurrentGameTimer -= 1;
		OnTimerUpdate.Broadcast(CurrentGameTimer);
	}

	if (CurrentGameTimer <= 0)
	{
		OnTimerEnded.Broadcast();
	}
}
