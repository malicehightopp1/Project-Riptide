// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CombatGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UCombatGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	UCombatGameplayAbility();
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = "Input")) FGameplayTag InputTag;
	
	UFUNCTION(BlueprintCallable, Category = "Damage") void ApplyDamageToTarget(AActor* TargetActor, float BaseDamage, bool bIsPowerDamage = false, UPARAM(meta = (Categories = "Affinity")) FGameplayTag AffinityTag = FGameplayTag());
};
