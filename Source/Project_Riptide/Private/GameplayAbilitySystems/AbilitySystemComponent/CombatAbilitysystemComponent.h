// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CombatAbilitysystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UCombatAbilitysystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
};
