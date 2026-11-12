// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "StatRowWidget.generated.h"

/**
 * 
 */
UCLASS()
class UStatRowWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	//Which stat this row shows - set on each row you place in WBP_StatsMenu
	UPROPERTY(EditAnywhere, Category = "Stat Row") FGameplayAttribute Stat;
	
	//Updates the number and turns the + button on/off
	void RefreshRow(class UAbilitySystemComponent* ASC, bool bCanLevelUp);

protected:
	virtual void NativePreConstruct() override;	//runs in the designer too - shows the stat name
	virtual void NativeOnInitialized() override;	//runs once - bind the button
	
	//----------------------------------------------------------------------------------//
	//					Widgets - names MUST match WBP_StatRow exactly
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> StatNameText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> StatValueText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> LevelUpButton;

private:
	UFUNCTION() void HandleLevelUpClicked();
};
