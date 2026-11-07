// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //this super is required because the base class constructor has it as well
{
	//false till we do movement
	PrimaryActorTick.bCanEverTick = false;
	
	//----------------------------------------------------------------------------------//
	//									Player rotation
	//----------------------------------------------------------------------------------//
	
	//Setting rotation settings
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//----------------------------------------------------------------------------------//
	//									Camera Setup
	//----------------------------------------------------------------------------------//

	//Camera Boom setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultTargetArmLength;
	CameraBoom->TargetOffset = FVector(DefaultCameraOffset);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = DefaultCameraLagSpeed;

	//Camera Component setup
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLookInput);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMoveInput);
	}
}

void APlayerCharacter::HandleLookInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputAction = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputAction.X);
	AddControllerPitchInput(InputAction.Y);
}

void APlayerCharacter::HandleMoveInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputAction = InputActionValue.Get<FVector2D>();
	
	InputAction.Normalize();
	AddMovementInput(GetMoveFWDdirection()* InputAction.Y + GetRightDirection() * InputAction.X);
}

FVector APlayerCharacter::GetRightDirection() const
{
	return CameraComponent->GetRightVector();
}

FVector APlayerCharacter::GetLookFWDVector() const
{
	return CameraComponent->GetForwardVector();
}

FVector APlayerCharacter::GetMoveFWDdirection() const
{
	return FVector::CrossProduct(GetRightDirection(), FVector::UpVector);
}
