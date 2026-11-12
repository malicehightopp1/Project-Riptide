// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Widgets/StatRowWidget.h"

#include "AbilitySystemComponent.h"
#include "StatsMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#pragma region Setup
void UStatRowWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	//Use the attribute's name as the label (e.g. "Vitality")
	if (StatNameText && Stat.IsValid())
	{
		StatNameText->SetText(FText::FromString(Stat.GetName()));
	}
}

void UStatRowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	LevelUpButton->OnClicked.AddDynamic(this, &UStatRowWidget::HandleLevelUpClicked);
}
#pragma endregion Setup

#pragma region Row
void UStatRowWidget::RefreshRow(UAbilitySystemComponent* ASC, bool bCanLevelUp)
{
	if (!ASC || !Stat.IsValid())
	{
		return;
	}
	
	//Current value of this stat
	StatValueText->SetText(FText::AsNumber(FMath::RoundToInt(ASC->GetNumericAttribute(Stat))));
	
	//Grey out the + if there isn't enough XP
	LevelUpButton->SetIsEnabled(bCanLevelUp);
}

void UStatRowWidget::HandleLevelUpClicked()
{
	//The row sits inside the stats menu - tell it which stat to level
	if (UStatsMenuWidget* Menu = GetTypedOuter<UStatsMenuWidget>())
	{
		Menu->LevelUpStat(Stat);
	}
}
#pragma endregion Row