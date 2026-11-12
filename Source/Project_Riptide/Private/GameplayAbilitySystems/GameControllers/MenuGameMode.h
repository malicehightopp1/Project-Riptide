// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

UCLASS()
class AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
		
public:
	//----------------------------------------------------------------------------------//
	//										Menu
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(EditDefaultsOnly, Category = "Menu") TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;
	
	//Left empty on purpose , no spawning character in main menu
	virtual void RestartPlayer(AController* NewPlayer) override {}

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY() TObjectPtr<class UMainMenuWidget> MainMenuWidget;
};
