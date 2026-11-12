// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "StatsMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UStatsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Re-reads every number from the player - called when opened and after leveling up
	void RefreshMenu();
	
	//Called by a stat row's + button
	void LevelUpStat(const FGameplayAttribute& Stat);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
protected:
	virtual void NativeOnInitialized() override;
	//----------------------------------------------------------------------------------//
	//					Widgets - names MUST match WBP_StatsMenu exactly
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> LevelText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> XPText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> NextLevelCostText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> DerivedStatsText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> StatPointsText;
private:
	//Every stat row placed in the menu (found automatically)
	UPROPERTY() TArray<TObjectPtr<class UStatRowWidget>> StatRows;
};
