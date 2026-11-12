// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Widgets/ClassInformationWidget.h"

#include "MainMenuWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameplayAbilitySystems/AttributeSets/CombatAttributeSet.h"
#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"

#pragma region Setup
void UClassInformationWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	RefreshCard();
}

void UClassInformationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SelectButton->OnClicked.AddDynamic(this, &UClassInformationWidget::HandleSelectClicked);
}
#pragma endregion Setup

#pragma region Selection
void UClassInformationWidget::HandleSelectClicked()
{
	//The card sits inside the main menu - find it and tell it this class was picked
	if (UMainMenuWidget* Menu = GetTypedOuter<UMainMenuWidget>())
	{
		Menu->SelectClass(CardClass);
	}
}

void UClassInformationWidget::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;
	RefreshCard();
}
#pragma endregion Selection

#pragma region Card Info
void UClassInformationWidget::RefreshCard()
{
	//Widgets can be missing for a moment in the designer
	if (!ClassNameText || !DescriptionText || !StatsText || !CardBorder)
	{
		return;
	}
	
	//----------------------------------------------------------------------------------//
	//						Name + description (from the design doc)
	//----------------------------------------------------------------------------------//
	
	FString ClassName;
	FString Description;
	
	switch (CardClass)
	{
		case ECharacterClass::Warrior:
			ClassName = TEXT("Warrior");
			Description = TEXT("Good with weapons.\n\nAffinity: Fire");
			break;
		case ECharacterClass::Tank:
			ClassName = TEXT("Tank");
			Description = TEXT("Great vitality, better with heavy weapons.\n\nAffinity: Earth");
			break;
		case ECharacterClass::Mage:
			ClassName = TEXT("Mage");
			Description = TEXT("Uses long range magic instead of basic weapons.\n\nAffinity: Wind");
			break;
		case ECharacterClass::Assassin:
			ClassName = TEXT("Assassin");
			Description = TEXT("Built for speed and fast attacks, better with light weapons.\n\nAffinity: Lightning");
			break;
		case ECharacterClass::Elementalist:
			ClassName = TEXT("Elementalist");
			Description = TEXT("Uses every power well, but deals less damage with weapons.\n\nAffinity: All");
			break;
	}
	
	ClassNameText->SetText(FText::FromString(ClassName));
	DescriptionText->SetText(FText::FromString(Description));
	
	//----------------------------------------------------------------------------------//
	//						Stats (read from the class GE)
	//----------------------------------------------------------------------------------//
	
	const FString Stats = FString::Printf(TEXT("Vitality: %.0f\nEndurance: %.0f\nMind: %.0f"),GetStartingStat(UCombatAttributeSet::GetVitalityAttribute()),GetStartingStat(UCombatAttributeSet::GetEnduranceAttribute()),GetStartingStat(UCombatAttributeSet::GetMindAttribute()));
	
	StatsText->SetText(FText::FromString(Stats));
	
	//Highlight
	CardBorder->SetBrushColor(bIsSelected ? SelectedColor : NormalColor);
}

float UClassInformationWidget::GetStartingStat(const FGameplayAttribute& Stat) const
{
	//Base value - what the attribute set constructor starts every stat at (1)
	float Value = Stat.GetNumericValue(GetDefault<UCombatAttributeSet>());
	
	if (!PlayerCharacterClass)
	{
		return Value;
	}
	
	//Find this class's GE in BP_PlayerCharacter's Class Effects map
	const APlayerCharacter* PlayerDefaults = PlayerCharacterClass->GetDefaultObject<APlayerCharacter>();
	const TSubclassOf<UGameplayEffect>* ClassEffect = PlayerDefaults->ClassEffects.Find(CardClass);
	if (!ClassEffect || !*ClassEffect)
	{
		return Value;
	}
	
	//Add up every modifier in the GE that changes this stat
	const UGameplayEffect* Effect = (*ClassEffect)->GetDefaultObject<UGameplayEffect>();
	for (const FGameplayModifierInfo& Modifier : Effect->Modifiers)
	{
		if (Modifier.Attribute != Stat)
		{
			continue;
		}
		
		//Reads the number typed into the modifier
		float Bonus = 0.0f;
		if (Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.0f, Bonus))
		{
			Value += Bonus;
		}
	}
	
	return Value;
}
#pragma endregion Card Info