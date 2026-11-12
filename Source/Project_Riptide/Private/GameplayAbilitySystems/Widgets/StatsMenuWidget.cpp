// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Widgets/StatsMenuWidget.h"

#include "StatRowWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"

#pragma region Setup
void UStatsMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetIsFocusable(true);
	//Find every stat row in the layout
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UStatRowWidget* Row = Cast<UStatRowWidget>(Widget))
		{
			StatRows.Add(Row);
		}
	});
}

FReply UStatsMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab || InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
		{
			Player->ToggleStatsMenu();
		}
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
#pragma endregion Setup

#pragma region Menu
void UStatsMenuWidget::RefreshMenu()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}
	
	//----------------------------------------------------------------------------------//
	//									Level + XP
	//----------------------------------------------------------------------------------//
	
	const float Level = ASC->GetNumericAttribute(UCombatAttributeSet::GetCharacterLevelAttribute());
	const float XP = ASC->GetNumericAttribute(UCombatAttributeSet::GetXPAttribute());
	const float Cost = Player->GetXPCostForNextLevel();
	const float Points = ASC->GetNumericAttribute(UCombatAttributeSet::GetStatPointsAttribute());
	
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level %.0f"), Level)));
	XPText->SetText(FText::FromString(FString::Printf(TEXT("XP: %.0f / %.0f"), XP, Cost)));
	NextLevelCostText->SetText(FText::FromString(FString::Printf(TEXT("Next level in %.0f XP"), FMath::Max(Cost - XP, 0.0f))));
	StatPointsText->SetText(FText::FromString(FString::Printf(TEXT("Stat Points: %.0f"), Points)));
	
	//----------------------------------------------------------------------------------//
	//						What your stats give you (max pools)
	//----------------------------------------------------------------------------------//
	
	DerivedStatsText->SetText(FText::FromString(FString::Printf(TEXT("Max Health: %.0f\nMax Stamina: %.0f\nMax Mana: %.0f"),ASC->GetNumericAttribute(UCombatAttributeSet::GetMaxHealthAttribute()),ASC->GetNumericAttribute(UCombatAttributeSet::GetMaxStaminaAttribute()),ASC->GetNumericAttribute(UCombatAttributeSet::GetMaxManaAttribute()))));
	
	//----------------------------------------------------------------------------------//
	//									Stat rows
	//----------------------------------------------------------------------------------//
	
	const bool bHasPoints = Points >= 1.0f;
	for (UStatRowWidget* Row : StatRows)
	{
		Row->RefreshRow(ASC, bHasPoints);
	}
}

void UStatsMenuWidget::LevelUpStat(const FGameplayAttribute& Stat)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (!Player)
	{
		return;
	}
	
	//Spends the XP and raises the stat + level (the function you already have)
	if (Player->TrySpendStatPoint(Stat))
	{
		RefreshMenu();
	}
}
#pragma endregion Menu