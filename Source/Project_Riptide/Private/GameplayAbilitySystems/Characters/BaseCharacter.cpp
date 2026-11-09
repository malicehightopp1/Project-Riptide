// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "CharacterMovement/BaseMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Init Setup
// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//----------------------------------------------------------------------------------//
	//							Ability system component Setup
	//----------------------------------------------------------------------------------//
	
	//Ability system Component
	CombatAbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitysystemComponent>(TEXT("CombatAbilitySystemComponent"));
	
	//----------------------------------------------------------------------------------//
	//									Movement Setup
	//----------------------------------------------------------------------------------//
	
	//Character Movement
	//this is setting the character movement to be our custom one
	BaseMovementComponent = CastChecked<UBaseMovementComponent>(GetMovementComponent()); //The castChecked will let you know immediatly if the cast fails instead of failing silently 
	
	//Initial Capsule size
	GetCapsuleComponent()->InitCapsuleSize(CapsuleInitWidth,CapsuleInitHeight);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
#pragma endregion Init Setup

#pragma region On Possession
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//NOTE: this is for initting the abilties and actor info, the owner and the avatar
	if (CombatAbilitySystemComponent)
	{
		CombatAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (CombatAbilitySystemComponent)
	{
		CombatAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

#pragma endregion On Possession

#pragma region Getters and setters
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return CombatAbilitySystemComponent;
}
#pragma endregion Getters and setters
