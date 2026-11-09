// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystems/Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	//----------------------------------------------------------------------------------//
	//									Camera setup
	//----------------------------------------------------------------------------------//
	
	//Camera Boom
	UPROPERTY(EditDefaultsOnly, Category = "Camera | Boom") TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, Category = "Camera | Boom") float DefaultTargetArmLength = 450.0f;
	UPROPERTY(EditAnywhere, Category = "Camera | Boom") FVector DefaultCameraOffset = FVector(0.0f, 0.0f, 50.0f);
	UPROPERTY(EditAnywhere, Category = "Camera | Boom") float DefaultCameraLagSpeed = 10.0f;

	//Camera Component
	UPROPERTY(EditDefaultsOnly, Category = "Camera") TObjectPtr<class UCameraComponent> CameraComponent; //for component classes use the TObjectPtr - unreal preferred
	
private:
	
	//----------------------------------------------------------------------------------//
	//							Player movement & input
	//----------------------------------------------------------------------------------//
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input
	virtual void BeginPlay() override;

	//Player movement & look functions
	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);
	void HandleJumpInput(const struct FInputActionValue& InputActionValue);
	
	//Getters for movement
	FVector GetRightDirection() const;
	FVector GetLookFWDVector() const;
	FVector GetMoveFWDdirection() const;
	
	//Mapping context
	UPROPERTY(EditDefaultsOnly, Category = "Input") class UInputMappingContext* PlayerInputMappingContext;
	
	//Input actions
	UPROPERTY(EditDefaultsOnly, Category = "Input") class UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") class UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") class UInputAction* JumpAction;
};
