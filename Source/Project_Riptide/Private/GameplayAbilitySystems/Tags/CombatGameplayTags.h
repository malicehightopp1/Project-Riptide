// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace CombatTags
{
	//States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	
	//Classes
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Class_Warrior);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Class_Tank);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Class_Mage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Class_Assassin);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Class_Elementalist);
	
	//Power / Affinity
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Affinity_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Affinity_Earth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Affinity_Wind);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Affinity_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Affinity_Water);
	
	//Inputs
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Attack_Light);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Attack_Heavy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Dodge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Power);
}