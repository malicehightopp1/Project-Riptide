// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Tags/CombatGameplayTags.h"
#include "NativeGameplayTags.h"

namespace CombatTags
{
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead");
	
	UE_DEFINE_GAMEPLAY_TAG(Class_Warrior, "Class.Warrior");
	UE_DEFINE_GAMEPLAY_TAG(Class_Tank, "Class.Tank");
	UE_DEFINE_GAMEPLAY_TAG(Class_Mage, "Class.Mage");
	UE_DEFINE_GAMEPLAY_TAG(Class_Assassin, "Class.Assassin");
	UE_DEFINE_GAMEPLAY_TAG(Class_Elementalist, "Class.Elementalist");
	
	UE_DEFINE_GAMEPLAY_TAG(Affinity_Fire, "Affinity.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Affinity_Earth, "Affinity.Earth");
	UE_DEFINE_GAMEPLAY_TAG(Affinity_Wind, "Affinity.Wind");
	UE_DEFINE_GAMEPLAY_TAG(Affinity_Lightning, "Affinity.Lightning");
	UE_DEFINE_GAMEPLAY_TAG(Affinity_Water, "Affinity.Water");
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Attack_Light, "Input.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Input_Attack_Heavy, "Input.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Input_Dodge, "Input.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Input_Power, "Input.Power");
	
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
}