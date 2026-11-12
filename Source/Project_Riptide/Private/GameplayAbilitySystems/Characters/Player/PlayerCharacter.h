// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystems/Characters/BaseCharacter.h"
#include "GameplayAbilitySystems/GameControllers/CombatGameInstance.h"
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
	
	//----------------------------------------------------------------------------------//
	//										UI
	//----------------------------------------------------------------------------------//
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<class UPlayerHUDWidget> PlayerHUDClass;
	void ToggleStatsMenu();
protected:
	virtual void OnAbilitySystemInitialized() override;
	
private:
	UPROPERTY() TObjectPtr<UPlayerHUDWidget> PlayerHUD;
	
	//Stats menu
	bool bStatsMenuOpen = false;
	
	//----------------------------------------------------------------------------------//
	//										Classes
	//----------------------------------------------------------------------------------//
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Effects") TMap<ECharacterClass, TSubclassOf<UGameplayEffect>> ClassEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<class UStatsMenuWidget> StatsMenuClass;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input") class UInputAction* StatsMenuAction;
	void HandleStatsMenuInput(const struct FInputActionValue& InputActionValue);
	
	UPROPERTY() TObjectPtr<UStatsMenuWidget> StatsMenu;
protected:   
	virtual TSubclassOf<UGameplayEffect> GetStatsEffect() const override;
	
	//----------------------------------------------------------------------------------//
	//							Player Progression and xp
	//----------------------------------------------------------------------------------//
public:
	UFUNCTION(BlueprintCallable, Category = "Leveling") bool TrySpendStatPoint(FGameplayAttribute Stat);
	UFUNCTION(BlueprintPure, Category = "Leveling") float GetXPCostForNextLevel() const;
	
	//Stat points you get each time you level up
	UPROPERTY(EditDefaultsOnly, Category = "Leveling") float StatPointsPerLevel = 1.0f;

private:
	void HandleXPChanged(const FOnAttributeChangeData& Data);
	void CheckForLevelUp();
	bool bIsLevelingUp = false; //stops CheckForLevelUp from running inside itself
	
	//----------------------------------------------------------------------------------//
	//							Player movement & input
	//----------------------------------------------------------------------------------//
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input
	virtual void BeginPlay() override;

	//Player movement & look functions
	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);
	void HandleJumpInput(const struct FInputActionValue& InputActionValue);
	
	//Abilties inputs
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = "Input")) TMap<TObjectPtr<class UInputAction>, FGameplayTag> AbilityInputActions;

	//Ability input pressed
	void HandleAbilityInputPressed(FGameplayTag InputTag);
	void HandleAbilityInputReleased(FGameplayTag InputTag);
	
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
