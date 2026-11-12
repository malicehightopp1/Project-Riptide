// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayAbilitySystems/AttributeSets/CombatAttributeSet.h"
#include "PlayerHUDWidget.generated.h"

UCLASS()
class UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Called by the player once the ASC has its stats 
	void InitFromAbilitySystem(class UAbilitySystemComponent* InASC);

protected:
	virtual void NativeDestruct() override;

	//Player bars
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<class UProgressBar> StaminaBar;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<class UProgressBar> ManaBar;

private:
	//for changing the ui
	void OnVitalChanged(const FOnAttributeChangeData& Data);
	void RefreshBars();

	TWeakObjectPtr<UAbilitySystemComponent> ASC;

	//Stored so we can unbind when the widget is destroyed
	TArray<TPair<FGameplayAttribute, FDelegateHandle>> BoundDelegates;
};
