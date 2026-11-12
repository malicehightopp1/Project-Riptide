// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/GameplayAbilities/CombatGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySystems/AttributeSets/CombatAttributeSet.h"
#include "GameplayAbilitySystems/Characters/BaseCharacter.h"

UCombatGameplayAbility::UCombatGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCombatGameplayAbility::ApplyDamageToTarget(AActor* TargetActor, float BaseDamage, bool bIsPowerDamage,FGameplayTag AffinityTag)
{
    UAbilitySystemComponent* AttackerASC = GetAbilitySystemComponentFromActorInfo();
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    //No ASC = can't be damaged.
    if (!AttackerASC || !TargetASC || AttackerASC == TargetASC)
    {
		return;
    }

    //Don't keep hitting
    if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
    {
		return;
    }

    //The damage effect
    const ABaseCharacter* Attacker = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (!Attacker || !Attacker->DamageEffect)
    {
		return;
    }
	
    const FGameplayAttribute MultiplierAttribute = bIsPowerDamage ? UCombatAttributeSet::GetPowerDamageMultiplierAttribute() : UCombatAttributeSet::GetWeaponDamageMultiplierAttribute();

    float FinalDamage = BaseDamage * AttackerASC->GetNumericAttribute(MultiplierAttribute);

    //This gives the affinity bonus
    if (AffinityTag.IsValid() && AttackerASC->HasMatchingGameplayTag(AffinityTag))
    {
        FinalDamage *= 1.25f;
    }

	//TODO: this is temp remember to REMOVE this
    UE_LOG(LogTemp, Warning, TEXT("------[DAMAGE] Base %.0f -> Final %.1f------"), BaseDamage, FinalDamage);
    
    //"Outgoing spec" = a copy made BY this ability, so it remembers who attacked (for XP)
    FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(Attacker->DamageEffect, GetAbilityLevel());
    if (!DamageSpec.IsValid()) return;

    //Fill the Data.Damage blank with our number
    DamageSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), FinalDamage);

    //Send it. GE_Damage puts FinalDamage into the target's IncomingDamage,
    //and the attribute set takes it from there (Health -> death -> XP)
    AttackerASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data.Get(), TargetASC);
}
