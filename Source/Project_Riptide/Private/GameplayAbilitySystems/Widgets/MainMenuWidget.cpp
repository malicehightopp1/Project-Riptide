// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Widgets/MainMenuWidget.h"

#include "ClassInformationWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

#pragma region Setup
void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	//Bind buttons
	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandlePlayClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuitClicked);
	BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleBackClicked);
	StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleStartClicked);
	
	//Find every class card in the layout - add or remove cards without touching code
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UClassInformationWidget* Card = Cast<UClassInformationWidget>(Widget))
		{
			ClassCards.Add(Card);
		}
	});
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//Start on the main page
	MenuSwitcher->SetActiveWidgetIndex(0);
	
	//Highlight whatever class is already saved (Warrior by default)
	ECharacterClass SavedClass = ECharacterClass::Warrior;
	if (const UCombatGameInstance* GI = Cast<UCombatGameInstance>(GetGameInstance()))
	{
		SavedClass = GI->SelectedClass;
	}
	SelectClass(SavedClass);
}
#pragma endregion Setup

#pragma region Page Buttons
void UMainMenuWidget::HandlePlayClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(1); //go to class select
}

void UMainMenuWidget::HandleBackClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(0); //back to main page
}

void UMainMenuWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::HandleStartClicked()
{
	if (GameplayLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("No Gameplay Level set in WBP_MainMenu Class Defaults!"));
		return;
	}
	
	//Class is already saved in the game instance - it survives the level change
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameplayLevel);
}
#pragma endregion Page Buttons

#pragma region Class Select
void UMainMenuWidget::SelectClass(ECharacterClass NewClass)
{
	//Save it where the player reads it when the open world loads
	if (UCombatGameInstance* GI = Cast<UCombatGameInstance>(GetGameInstance()))
	{
		GI->SelectedClass = NewClass;
	}
	
	//Highlight the picked card, un-highlight the rest
	for (UClassInformationWidget* Card : ClassCards)
	{
		Card->SetSelected(Card->CardClass == NewClass);
	}
}
#pragma endregion Class Select
