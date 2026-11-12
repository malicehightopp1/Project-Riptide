// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CombatGameInstance.generated.h"

UENUM()
enum class ECharacterClass : uint8
{
	Warrior,
	Tank,
	Mage,
	Assassin,
	Elementalist
};

UCLASS()
class UCombatGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY() ECharacterClass SelectedClass = ECharacterClass::Warrior;
	
	//Note: this for calling the selected class later or betetr to tell the player and attribute set what gameplay effect to add
	/*if (UCombatGameInstance* GI = Cast<UCombatGameInstance>(GetGameInstance()))
	{
		GI->SelectedClass = ECharacterClass::Tank;
	}*/
};
