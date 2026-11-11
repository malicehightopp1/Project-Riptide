// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "CharacterMovement/BaseMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Init Setup
// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//----------------------------------------------------------------------------------//
	//							Ability system component Setup
	//----------------------------------------------------------------------------------//
	
	//Ability system Component
	CombatAbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitysystemComponent>(TEXT("CombatAbilitySystemComponent"));
	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));
	
	//----------------------------------------------------------------------------------//
	//									Movement Setup
	//----------------------------------------------------------------------------------//
	
	//Character Movement
	//this is setting the character movement to be our custom one
	BaseMovementComponent = CastChecked<UBaseMovementComponent>(GetMovementComponent()); //The castChecked will let you know immediatly if the cast fails instead of failing silently 
	
	//Initial Capsule size
	GetCapsuleComponent()->InitCapsuleSize(CapsuleInitWidth,CapsuleInitHeight);
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	if (!EffectClass || !CombatAbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle Context = CombatAbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec = CombatAbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, Context);
	if (Spec.IsValid())
	{
		CombatAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void ABaseCharacter::OnDeath(AActor* Killer)
{
	CombatAbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	GetCharacterMovement()->DisableMovement();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
#pragma endregion Init Setup

#pragma region On Possession
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystem();
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystem();
}

void ABaseCharacter::InitAbilitySystem()
{
	if (!CombatAbilitySystemComponent) return;

	UE_LOG(LogTemp, Warning, TEXT("InitAbilitySystem called on %s"), *GetName());
	
	//NOTE: this is for initting the abilties and actor info, the owner and the avatar
	CombatAbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		ApplyStartupEffects();
	}
}

void ABaseCharacter::ApplyStartupEffects()
{
	if (bStartupEffectsApplied || !CombatAbilitySystemComponent)
	{
		return;
	}
	
	bStartupEffectsApplied = true;

	ApplyEffectToSelf(GetStatsEffect(), StartupEffectLevel); //1. class stats (GE_Warrior)
	ApplyEffectToSelf(StartupEffects, StartupEffectLevel); //2. derived stats (GE_StatSetup)
	ApplyEffectToSelf(RefillVitalsEffect); //3. fill pools (GE_RefillStats)

	if (GEngine && CombatAttributeSet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(
			TEXT("HP %.0f / %.0f   |   Stamina %.0f / %.0f   |   Mana %.0f / %.0f"),
			CombatAttributeSet->GetHealth(),  CombatAttributeSet->GetMaxHealth(),
			CombatAttributeSet->GetStamina(), CombatAttributeSet->GetMaxStamina(),
			CombatAttributeSet->GetMana(),    CombatAttributeSet->GetMaxMana()));
	}
	
	OnAbilitySystemInitialized();

}

#pragma endregion On Possession

#pragma region Getters and setters
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return CombatAbilitySystemComponent;
}
#pragma endregion Getters and setters
