// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CallOfTsushimaCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitByProjectile);

UCLASS(config=Game)
class ACallOfTsushimaCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;


	
public:
	ACallOfTsushimaCharacter();
	
protected:
	virtual void BeginPlay();
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health", Replicated)
	float Health = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FString ColorTag = "";

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintAssignable)
	FOnHitByProjectile OnHitByProjectile;
	
	UPROPERTY()
	UTP_WeaponComponent* WeaponComponent;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;
	
	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);
	
	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(Server, Reliable)
	void OnFire();

	UFUNCTION(NetMulticast, Unreliable)
	void FireFX();

	UFUNCTION(NetMulticast, Unreliable)
	void UpdateColorTag();

	UFUNCTION(Server, Unreliable)
	void UpdateColorTagForEveryone();

	UFUNCTION()
	void Shoot();

	UPROPERTY(VisibleAnywhere)
	FVector InitialPosition;

	UPROPERTY(VisibleAnywhere)
	FRotator InitialRotation;
	
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void Fire(const FInputActionValue& Value);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
	
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
};

