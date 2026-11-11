// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Widgets/PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "GameplayAbilitySystems/AttributeSets/CombatAttributeSet.h"

void UPlayerHUDWidget::InitFromAbilitySystem(UAbilitySystemComponent* InASC)
{
    if (!InASC)
    {
        return;
    }
    ASC = InASC;

    //Grabbing all the attributes
    const FGameplayAttribute Vitals[] =
    {
        UCombatAttributeSet::GetHealthAttribute(),
        UCombatAttributeSet::GetMaxHealthAttribute(),
        UCombatAttributeSet::GetStaminaAttribute(), 
        UCombatAttributeSet::GetMaxStaminaAttribute(),
        UCombatAttributeSet::GetManaAttribute(),    
        UCombatAttributeSet::GetMaxManaAttribute()
    };

    //binding all attributes to on change event
    for (const FGameplayAttribute& Attribute : Vitals)
    {
        const FDelegateHandle Handle = InASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UPlayerHUDWidget::OnVitalChanged);
        BoundDelegates.Add(TPair<FGameplayAttribute, FDelegateHandle>(Attribute, Handle));
    }

    //Set the starting values right away
    RefreshBars();
}

void UPlayerHUDWidget::NativeDestruct()
{
    
    //Unbinding the widget 
    if (UAbilitySystemComponent* ASCPtr = ASC.Get())
    {
        for (const TPair<FGameplayAttribute, FDelegateHandle>& Bound : BoundDelegates)
        {
            ASCPtr->GetGameplayAttributeValueChangeDelegate(Bound.Key).Remove(Bound.Value);
        }
    }
    BoundDelegates.Empty();

    Super::NativeDestruct();
}

void UPlayerHUDWidget::OnVitalChanged(const FOnAttributeChangeData& Data)
{
    //Refeshing the ui and bars for example if the player upgrades there vitality stat
    RefreshBars();
}

void UPlayerHUDWidget::RefreshBars()
{
    UE_LOG(LogTemp, Warning, TEXT("Player stat bars have been refreshed"));
    
    UAbilitySystemComponent* ASCPtr = ASC.Get();
    if (!ASCPtr)
    {
        return;
    }

    //keeping the value between 0 and 1
    auto GetPercent = [ASCPtr](const FGameplayAttribute& Current, const FGameplayAttribute& Max)
    {
        const float MaxValue = ASCPtr->GetNumericAttribute(Max);
        return MaxValue > 0.0f ? ASCPtr->GetNumericAttribute(Current) / MaxValue : 0.0f;
    };
    
    //Actually setting the information
    HealthBar->SetPercent(GetPercent(UCombatAttributeSet::GetHealthAttribute(), UCombatAttributeSet::GetMaxHealthAttribute()));
    if (StaminaBar)
    {
        StaminaBar->SetPercent(GetPercent(UCombatAttributeSet::GetStaminaAttribute(), UCombatAttributeSet::GetMaxStaminaAttribute()));
    }
    if (ManaBar)
    {
        ManaBar->SetPercent(GetPercent(UCombatAttributeSet::GetManaAttribute(), UCombatAttributeSet::GetMaxManaAttribute()));
    }
}