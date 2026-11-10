// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
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
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	//----------------------------------------------------------------------------------//
	//									Camera Setup
	//----------------------------------------------------------------------------------//

	//Camera Boom setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	//Arm Length
	CameraBoom->TargetArmLength = DefaultTargetArmLength;
	CameraBoom->TargetOffset = FVector(DefaultCameraOffset);
	
	//Rotation
	CameraBoom->bUsePawnControlRotation = true;
	
	//Camera Lag
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = DefaultCameraLagSpeed;
	

	//Camera Component setup
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
}

#pragma region Player Locomotion/Input Actions
void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		//Player Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLookInput);
		
		//Player Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMoveInput);
		
		UE_LOG(LogTemp, Warning, TEXT("Input Actions Have been binded!"))
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::HandleLookInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputAction = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputAction.X);
	AddControllerPitchInput(InputAction.Y);
	
	UE_LOG(LogTemp, Warning, TEXT("Look Input is being called by the look action input"))
}

void APlayerCharacter::HandleMoveInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputAction = InputActionValue.Get<FVector2D>();
	
	InputAction.Normalize();
	AddMovementInput(GetMoveFWDdirection()* InputAction.Y + GetRightDirection() * InputAction.X);
	
	UE_LOG(LogTemp, Warning, TEXT("Move Input is happening"))
}
#pragma endregion Player Locomotion/Input Actions

#pragma region Getters

//----------------------------------------------------------------------------------//
//									Locomotion Getters
//----------------------------------------------------------------------------------//

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
#pragma endregion Getters
