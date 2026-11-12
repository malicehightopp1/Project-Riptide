// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"
#include "GameplayAbilitySystems/GameControllers/CombatGameInstance.h"
#include "ClassInformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class UClassInformationWidget : public UUserWidget
{
		GENERATED_BODY()
	
public:
	//Which class this card shows - set on each card you place in WBP_MainMenu
	UPROPERTY(EditAnywhere, Category = "Class Card") ECharacterClass CardClass = ECharacterClass::Warrior;
	
	//Turns the highlight on/off - the menu calls this
	void SetSelected(bool bSelected);

protected:
	virtual void NativePreConstruct() override;	//runs in the designer too - so cards preview their info
	virtual void NativeOnInitialized() override;//runs once - good for binding the button
	
	//----------------------------------------------------------------------------------//
	//									Card settings
	//----------------------------------------------------------------------------------//
	
	//Stats are read from this BP's Class Effects map - set once in WBP_ClassCard Class Defaults
	UPROPERTY(EditDefaultsOnly, Category = "Class Card") TSubclassOf<APlayerCharacter> PlayerCharacterClass;
	
	//Background colors
	UPROPERTY(EditDefaultsOnly, Category = "Class Card") FLinearColor NormalColor = FLinearColor(0.02f, 0.02f, 0.02f, 0.85f);
	UPROPERTY(EditDefaultsOnly, Category = "Class Card") FLinearColor SelectedColor = FLinearColor(0.55f, 0.42f, 0.12f, 0.9f);
	
	//----------------------------------------------------------------------------------//
	//					Widgets - names MUST match WBP_ClassCard exactly
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> SelectButton;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UBorder> CardBorder;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> ClassNameText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> DescriptionText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> StatsText;

private:
	UFUNCTION() void HandleSelectClicked();
	
	//Fills in the text and background color
	void RefreshCard();
	
	//Base stat (1) + this class's bonus from its GE
	float GetStartingStat(const FGameplayAttribute& Stat) const;
	
	bool bIsSelected = false;
};
