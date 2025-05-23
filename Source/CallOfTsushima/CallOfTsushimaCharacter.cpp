// Copyright Epic Games, Inc. All Rights Reserved.

#include "CallOfTsushimaCharacter.h"
#include "CallOfTsushimaProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// ACallOfTsushimaCharacter

ACallOfTsushimaCharacter::ACallOfTsushimaCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ACallOfTsushimaCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	InitialPosition = GetActorLocation();
	InitialRotation = GetActorRotation();
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACallOfTsushimaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACallOfTsushimaCharacter, ColorTag);
	DOREPLIFETIME(ACallOfTsushimaCharacter, Health);
}

//////////////////////////////////////////////////////////////////////////// Input

void ACallOfTsushimaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACallOfTsushimaCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACallOfTsushimaCharacter::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACallOfTsushimaCharacter::Fire);
	}
	else
	{
		
		// UE_LOG(, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ACallOfTsushimaCharacter::FireFX_Implementation()
{
	WeaponComponent->HandleProjectileFX();
}

void ACallOfTsushimaCharacter::UpdateColorTag_Implementation()
{
	// if (ColorTag.IsEmpty())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			APlayerState* CurrState = PlayerController->PlayerState;

			if (CurrState == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("STATE NON TROVATO"));
			}
			else
			{
				int32 UniqID = CurrState->GetPlayerId();
				if (UniqID % 2 == 0)
				{
					ColorTag = "Blue";
				}
				else
				{
					ColorTag =  "Red";
				}
				
			}
		}
	}
}

void ACallOfTsushimaCharacter::UpdateColorTagForEveryone_Implementation()
{
	UpdateColorTag();
}

void ACallOfTsushimaCharacter::Shoot()
{
	if (WeaponComponent == nullptr) return;

	WeaponComponent->HandleProjectile();
	FireFX();
}

void ACallOfTsushimaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACallOfTsushimaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACallOfTsushimaCharacter::Fire(const FInputActionValue& Value)
{
	if (GetLocalRole()  == ROLE_Authority)
	{
		Shoot();
		UpdateColorTag();
	}
	else
	{
		OnFire();
		UpdateColorTagForEveryone();
	}
}

void ACallOfTsushimaCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ACallOfTsushimaCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ACallOfTsushimaCharacter::OnFire_Implementation()
{
	Shoot();
}
