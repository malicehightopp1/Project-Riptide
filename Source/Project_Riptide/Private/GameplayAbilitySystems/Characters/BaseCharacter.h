// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySystems/AbilitySystemComponent/CombatAbilitysystemComponent.h"
#include "GameplayAbilitySystems/AttributeSets/CombatAttributeSet.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);// Sets default values for this character's properties

	//----------------------------------------------------------------------------------//
	//									Ability system component
	//----------------------------------------------------------------------------------//
	
	//Ability system component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities") class UCombatAbilitysystemComponent* CombatAbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities") TObjectPtr<UCombatAttributeSet> CombatAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities") TSubclassOf<UGameplayEffect> StartupEffects;
	UPROPERTY(EditAnywhere, Category = "Abilities") float StartupEffectLevel = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities") TSubclassOf<UGameplayEffect> RefillVitalsEffect;


	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level = 1.0f);
	virtual void OnDeath(AActor* Killer);
	//----------------------------------------------------------------------------------//
	//									Player movement
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterMovement") class UBaseMovementComponent* BaseMovementComponent;

	//----------------------------------------------------------------------------------//
	//									Basic setup
	//----------------------------------------------------------------------------------//
	
	//Capsule Default Variables
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement") float CapsuleInitWidth = 35.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement") float CapsuleInitHeight = 90.0f;
	
protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	//Ability setup
	void InitAbilitySystem();
	void ApplyStartupEffects();
	bool bStartupEffectsApplied = false;
	
	virtual void OnAbilitySystemInitialized() {}
	
	virtual TSubclassOf<UGameplayEffect> GetStatsEffect() const { return nullptr; }
	
public:	
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override; //grabbing the ability system component for any child of this class
};
