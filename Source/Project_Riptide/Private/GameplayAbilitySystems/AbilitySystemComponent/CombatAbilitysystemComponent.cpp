// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/AbilitySystemComponent/CombatAbilitysystemComponent.h"

#include "GameplayAbilitySystems/GameplayAbilities/CombatGameplayAbility.h"

void UCombatAbilitysystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	//Collect first, activate after - activating can change the ability list mid-loop
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		const UCombatGameplayAbility* CombatAbility = Cast<UCombatGameplayAbility>(Spec.Ability);
		if (!CombatAbility || !CombatAbility->InputTag.MatchesTagExact(InputTag)) continue;

		AbilitySpecInputPressed(Spec); //lets an already-running ability know input was pressed

		if (!Spec.IsActive())
		{
			AbilitiesToActivate.Add(Spec.Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToActivate)
	{
		TryActivateAbility(Handle);
	}
}

void UCombatAbilitysystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		const UCombatGameplayAbility* CombatAbility = Cast<UCombatGameplayAbility>(Spec.Ability);
		if (CombatAbility && CombatAbility->InputTag.MatchesTagExact(InputTag))
		{
			AbilitySpecInputReleased(Spec); //for charge/hold abilities later
		}
	}
}