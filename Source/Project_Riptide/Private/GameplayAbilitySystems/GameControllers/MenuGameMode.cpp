// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/GameControllers/MenuGameMode.h"

#include "GameplayAbilitySystems/Widgets/MainMenuWidget.h"

#pragma region Menu Setup
void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !MainMenuWidgetClass)
	{
		return;
	}
	
	//Create the menu and put it on screen
	MainMenuWidget = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass);
	if (!MainMenuWidget)
	{
		return;
	}
	MainMenuWidget->AddToViewport();
	
	//Mouse controls the menu, not the game
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
}
#pragma endregion Menu Setup