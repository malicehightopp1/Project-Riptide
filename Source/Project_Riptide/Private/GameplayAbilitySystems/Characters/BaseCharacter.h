// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySystems/AbilitySystemComponent/CombatAbilitysystemComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);// Sets default values for this character's properties

	//Ability system component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities") class UCombatAbilitysystemComponent* CombatAbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement") class UBaseMovementComponent* BaseMovementComponent;

	//Capsule Default Variables
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement") float CapsuleInitWidth = 25.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement") float CapsuleInitHeight = 90.0f;
	
protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input
	
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override; //grabbing the ability system component for any child of this class
};
