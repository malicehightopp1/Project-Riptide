// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CombatAttributeSet.generated.h"

//This generates a Get, Set, and init and static get Attribute for every attribute
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \
	
UCLASS()
class UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	 
public:
	UCombatAttributeSet();
	//These might give errors not setup yet
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	//Xp cost to buy the next level 2 * L^2.3 + 84
	static float GetXPCostForLevel(float CurrentLevel);
	
	//----------------------------------------------------------------------------------//
	//									Player Health
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Health") FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Health") FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxHealth)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stamina") FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Stamina)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stamina") FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxStamina)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Mana") FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Mana)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets  | Mana") FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxMana)
	
	//----------------------------------------------------------------------------------//
	//									Player Base Stats
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stats") FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Vitality)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stats") FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Endurance)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets  | Stats") FGameplayAttributeData Mind;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Mind)
	
	//----------------------------------------------------------------------------------//
	//									Player Progression stats
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stats") FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CharacterLevel)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stats") FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, XP)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets  | Stats") FGameplayAttributeData XPReward;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, XPReward)
	
	//----------------------------------------------------------------------------------//
	//									Combat Multipliers
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets | Stats") FGameplayAttributeData WeaponDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, WeaponDamageMultiplier)
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets  | Stats") FGameplayAttributeData PowerDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, PowerDamageMultiplier)
	
	//Temporary valie 
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSets  | Meta") FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, IncomingDamage)
	
private:
	void HandleDeath(const FGameplayEffectModCallbackData& Data);
};
