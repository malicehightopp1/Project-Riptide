// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystems/Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	
public:
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	//----------------------------------------------------------------------------------//
	//                         UI
	//----------------------------------------------------------------------------------//
	
	UPROPERTY(VisibleAnywhere, Category = "UI") TObjectPtr<class UWidgetComponent> HealthBarWidget;
	
	//----------------------------------------------------------------------------------//
	//                         Stats
	//----------------------------------------------------------------------------------//
    
	//This enemy's stats + XP reward 
	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Effects") TSubclassOf<UGameplayEffect> EnemyStatsEffect;

	//----------------------------------------------------------------------------------//
	//                         Death
	//----------------------------------------------------------------------------------//
    
	//Seconds the body stays before being removed
	UPROPERTY(EditDefaultsOnly, Category = "Death") float CorpseLifeSpan = 5.0f;

	virtual void OnDeath(AActor* Killer) override;

protected:
	//Hands EnemyStatsEffect to BaseCharacter's startup chain
	virtual TSubclassOf<UGameplayEffect> GetStatsEffect() const override;
	
	virtual void BeginPlay() override;
};
