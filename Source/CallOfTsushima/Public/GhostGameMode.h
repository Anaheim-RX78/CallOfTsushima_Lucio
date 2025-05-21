// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaintableSurface.h"
#include "GameFramework/GameMode.h"
#include "GhostGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecondPassed, int, TimerSeconds);

UCLASS()
class CALLOFTSUSHIMA_API AGhostGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimerInSeconds = 120;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentGameTimer = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentTimer = 0;

	UPROPERTY(BlueprintAssignable)
	FOnSecondPassed OnTimerUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnTimerEnded OnTimerEnded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Red;
	
	FLinearColor ConvertColor(TSharedPtr<FJsonValue>& JsonValue);
};
