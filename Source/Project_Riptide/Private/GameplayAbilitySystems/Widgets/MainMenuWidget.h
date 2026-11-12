// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayAbilitySystems/GameControllers/CombatGameInstance.h"
#include "MainMenuWidget.generated.h"

/**
 * Main menu + class select in one widget. A Widget Switcher flips between the two pages.
 */
UCLASS()
class UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//The map Start loads - pick it in WBP_MainMenu's Class Defaults
	UPROPERTY(EditDefaultsOnly, Category = "Menu") TSoftObjectPtr<UWorld> GameplayLevel;
	
	//Called by a class card when clicked - saves the class and highlights that card
	void SelectClass(ECharacterClass NewClass);

protected:
	virtual void NativeOnInitialized() override;	//runs once - bind buttons, find cards
	virtual void NativeConstruct() override;		//runs each time it's shown - reset the page
	
	//----------------------------------------------------------------------------------//
	//					Widgets - names MUST match WBP_MainMenu exactly
	//----------------------------------------------------------------------------------//
	
	//Flips pages: 0 = Main, 1 = Class Select
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UWidgetSwitcher> MenuSwitcher;
	
	//Main page
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> PlayButton;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> QuitButton;
	
	//Class select page (the cards are found automatically - no names needed)
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> BackButton;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> StartButton;

private:
	//----------------------------------------------------------------------------------//
	//									Button clicks
	//----------------------------------------------------------------------------------//
	
	UFUNCTION() void HandlePlayClicked();
	UFUNCTION() void HandleQuitClicked();
	UFUNCTION() void HandleBackClicked();
	UFUNCTION() void HandleStartClicked();
	
	//Every class card placed in the menu
	UPROPERTY() TArray<TObjectPtr<class UClassInformationWidget>> ClassCards;
};
