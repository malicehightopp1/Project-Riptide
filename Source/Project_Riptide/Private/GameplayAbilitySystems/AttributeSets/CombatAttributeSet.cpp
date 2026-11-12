// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/AttributeSets/CombatAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayAbilitySystems/Characters/BaseCharacter.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	//Initing all base values
	InitVitality(1.0f);
	InitEndurance(1.0f);
	InitMind(1.0f);
	InitCharacterLevel(1.0f);
	InitWeaponDamageMultiplier(1.0f);
	InitPowerDamageMultiplier(1.0f); 
}

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	//Clamping the values between 0 and 1
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UCombatAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	//if a max goes down (buff or such) pull the value down as well
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UCombatAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;
	
	if (Attribute == GetIncomingDamageAttribute())
	{
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.0f); //consumes damage and reset it
		
		UE_LOG(LogTemp, Warning, TEXT("[GAS] IncomingDamage %.0f -> Health was %.0f"), Damage, GetHealth()); //TEMP
		
		if (Damage <= 0.0f)
		{
			return;
		}
		const float OldHealth = GetHealth();
		const float NewHealth = FMath::Clamp(OldHealth - Damage, 0.0f, GetMaxHealth());
		SetHealth(NewHealth);
		
		//Player death called
		if (OldHealth > 0.0f && NewHealth <= 0.0f)
		{
			HandleDeath(Data);
		}
	}
	else if (Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}

float UCombatAttributeSet::GetXPCostForLevel(float CurrentLevel)
{
	return FMath::FloorToFloat(2.0f * FMath::Pow(CurrentLevel, 2.3f) + 84.0f);
}

void UCombatAttributeSet::HandleDeath(const FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEATH] HandleDeath on %s | XPReward %.0f"), *GetNameSafe(GetOwningActor()), GetXPReward()); //TEMP
	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	AActor* Killer = Context.GetOriginalInstigator();

	//Pay this character's XPReward to whoever killed it
	UAbilitySystemComponent* KillerASC = Context.GetOriginalInstigatorAbilitySystemComponent();
	if (KillerASC && KillerASC != GetOwningAbilitySystemComponent() && KillerASC->HasAttributeSetForAttribute(GetXPAttribute()) && GetXPReward() > 0.0f)
	{
		const float KillerXP = KillerASC->GetNumericAttributeBase(GetXPAttribute());
		KillerASC->SetNumericAttributeBase(GetXPAttribute(), KillerXP + GetXPReward());
	}

	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwningActor()))
	{
		OwnerCharacter->OnDeath(Killer);
	}
}
