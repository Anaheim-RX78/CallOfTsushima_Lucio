// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostGameMode.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AGhostGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameTimer = TimerInSeconds;

	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	// Request->SetURL("https://meowfacts.herokuapp.com/");
	Request->SetURL("https://x-colors.yurace.pro/api/random?number=2");
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", "application/json");
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr,FHttpResponsePtr Response,bool)
	{
		if (!Response.IsValid()) return;
		FString ResponseText = Response->GetContentAsString();

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseText);
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		
		if (FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			if (JsonArray[0] && JsonArray[1])
			{
				Blue = ConvertColor(JsonArray[0]);
				Red = ConvertColor(JsonArray[1]);
			}
		}
	});
	Request->ProcessRequest();
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

FLinearColor AGhostGameMode::ConvertColor(TSharedPtr<FJsonValue>& JsonValue)
{
	TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
	if (!Obj.IsValid()) return FLinearColor::White;

	FString RgbStr = Obj->GetStringField("rgb"); // e.g., "rgb(251, 37, 56)"

	// Strip to numbers
	RgbStr.RemoveFromStart("rgb(");
	RgbStr.RemoveFromEnd(")");
	TArray<FString> Channels;
	RgbStr.ParseIntoArray(Channels, TEXT(","), true);

	if (Channels.Num() == 3)
	{
		float R = FCString::Atof(*Channels[0].TrimStartAndEnd()) / 255.f;
		float G = FCString::Atof(*Channels[1].TrimStartAndEnd()) / 255.f;
		float B = FCString::Atof(*Channels[2].TrimStartAndEnd()) / 255.f;

		return FLinearColor(R, G, B, 1.0f);
	}

	return FLinearColor::White;
}
