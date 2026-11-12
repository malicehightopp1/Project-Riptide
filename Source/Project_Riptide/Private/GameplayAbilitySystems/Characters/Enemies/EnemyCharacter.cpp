// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Enemies/EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySystems/Widgets/PlayerHUDWidget.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Without this, spawned enemies never get an AI controller -> PossessedBy never runs -> no stats
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Enemies face the way they move (for when AI is added)
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(120.0f, 12.0f));
}

TSubclassOf<UGameplayEffect> AEnemyCharacter::GetStatsEffect() const
{
	return EnemyStatsEffect;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UPlayerHUDWidget* HealthBar = Cast<UPlayerHUDWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		HealthBar->InitFromAbilitySystem(GetAbilitySystemComponent());
	}
}

void AEnemyCharacter::OnDeath(AActor* Killer)
{
	UE_LOG(LogTemp, Warning, TEXT("[DEATH] EnemyCharacter::OnDeath on %s"), *GetName()); //TEMP
	Super::OnDeath(Killer); //adds State.Dead + stops movement

	HealthBarWidget->SetVisibility(false);
	
	//Stop the AI and stop blocking the player
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Ragdoll the body
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	SetLifeSpan(CorpseLifeSpan);
}
