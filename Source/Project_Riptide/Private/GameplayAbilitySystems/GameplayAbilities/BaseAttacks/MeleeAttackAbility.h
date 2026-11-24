// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystems/GameplayAbilities/CombatGameplayAbility.h"
#include "MeleeAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class UMeleeAttackAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack") TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Attack") float BaseDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack") float PlayRate = 1.0f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION() void OnMontageFinished();
	UFUNCTION() void OnMontageCancelled();
	UFUNCTION() void OnHitEvent(FGameplayEventData Payload);
};
