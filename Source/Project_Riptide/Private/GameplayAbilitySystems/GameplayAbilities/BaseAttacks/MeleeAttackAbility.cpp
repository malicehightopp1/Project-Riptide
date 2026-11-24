// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/GameplayAbilities/BaseAttacks/MeleeAttackAbility.h"
#include "GameplayAbilitySystems/Tags/CombatGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


void UMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("[ATTACK] %s activated"), *GetName()); //TEMP

	//No montage or can't pay the cost -> stop right away
	if (!AttackMontage || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//Listen for the hitbox saying "I hit something"
	UAbilityTask_WaitGameplayEvent* WaitHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent( this, CombatTags::Event_Melee_Hit, nullptr, /*OnlyTriggerOnce*/ false, /*OnlyMatchExact*/ true);
	WaitHit->EventReceived.AddDynamic(this, &UMeleeAttackAbility::OnHitEvent);
	WaitHit->ReadyForActivation();

	//Play the swing - the Melee Trace notify inside the montage turns the hitbox on and off
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy( this, NAME_None, AttackMontage, PlayRate);
	MontageTask->OnCompleted.AddDynamic(this, &UMeleeAttackAbility::OnMontageFinished);
	MontageTask->OnBlendOut.AddDynamic(this, &UMeleeAttackAbility::OnMontageFinished);
	MontageTask->OnInterrupted.AddDynamic(this, &UMeleeAttackAbility::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UMeleeAttackAbility::OnMontageCancelled);
	MontageTask->ReadyForActivation();
}

void UMeleeAttackAbility::OnHitEvent(FGameplayEventData Payload)
{
	//function does the multipliers, affinity, GE and XP-on-kill
	ApplyDamageToTarget(const_cast<AActor*>(Payload.Target.Get()), BaseDamage);
}

void UMeleeAttackAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMeleeAttackAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}