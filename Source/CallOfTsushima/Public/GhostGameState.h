// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaintableSurface.h"
#include "GameFramework/GameState.h"
#include "GhostGameState.generated.h"

/**
 * 
 */
UCLASS()
class CALLOFTSUSHIMA_API AGhostGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APaintableSurface*> Paintables = {};

	UFUNCTION(BlueprintCallable)
	TMap<EPaintColor, int> GetAllColors();
	
	virtual void BeginPlay() override;
	
};
