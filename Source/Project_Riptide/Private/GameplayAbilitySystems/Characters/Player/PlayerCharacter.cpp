// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilitySystems/Widgets/PlayerHUDWidget.h"
#include "GameplayAbilitySystems/Widgets/StatsMenuWidget.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //this super is required because the base class constructor has it as well
{
	//false till we do movement
	PrimaryActorTick.bCanEverTick = false;
	
	//----------------------------------------------------------------------------------//
	//									Player rotation
	//----------------------------------------------------------------------------------//
	
	//Setting rotation settings
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	//fixing camera rotation and allowing animations to work properly
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,720.0f,0.0f);
	
	//----------------------------------------------------------------------------------//
	//									Camera Setup
	//----------------------------------------------------------------------------------//

	//Camera Boom setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	//Arm Length
	CameraBoom->TargetArmLength = DefaultTargetArmLength;
	CameraBoom->TargetOffset = FVector(DefaultCameraOffset);
	
	//Rotation
	CameraBoom->bUsePawnControlRotation = true;
	
	//Camera Lag
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = DefaultCameraLagSpeed;
	

	//Camera Component setup
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
}

#pragma region Player UI
void APlayerCharacter::ToggleStatsMenu()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !StatsMenuClass)
	{
		return;
	}
	
	//Make the menu the first time it's opened
	if (!StatsMenu)
	{
		StatsMenu = CreateWidget<UStatsMenuWidget>(PC, StatsMenuClass);
	}
	if (!StatsMenu)
	{
		return;
	}
	
	if (bStatsMenuOpen)
	{
		//Close - back to playing
		StatsMenu->RemoveFromParent();
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		PC->SetIgnoreLookInput(false);
		bStatsMenuOpen = false;
	}
	else
	{
		//Open - menu gets focus so it can catch Tab/Escape to close
		StatsMenu->AddToViewport();
		StatsMenu->RefreshMenu();
		
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(StatsMenu->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->SetIgnoreLookInput(true);
		bStatsMenuOpen = true;
	}
}

void APlayerCharacter::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
	CombatAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetXPAttribute()).AddUObject(this, &APlayerCharacter::HandleXPChanged);
	
	CombatAttributeSet->SetXP(1000.0f); //for testing XP
	
	//Only the local player gets a HUD, and only once
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !PC->IsLocalController() || !PlayerHUDClass || PlayerHUD)
	{
		return;
	}

	PlayerHUD = CreateWidget<UPlayerHUDWidget>(PC, PlayerHUDClass);
	if (PlayerHUD)
	{
		PlayerHUD->AddToViewport();
		PlayerHUD->InitFromAbilitySystem(GetAbilitySystemComponent());
	}
}
#pragma endregion 

#pragma region Player Progression

float APlayerCharacter::GetXPCostForNextLevel() const
{
	if (!CombatAttributeSet)
	{
		return 0.0f;
	}
	return UCombatAttributeSet::GetXPCostForLevel(CombatAttributeSet->GetCharacterLevel());
}

void APlayerCharacter::HandleXPChanged(const FOnAttributeChangeData& Data)
{
	CheckForLevelUp();
}

void APlayerCharacter::CheckForLevelUp()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC || bIsLevelingUp)
	{
		return;
	}
	bIsLevelingUp = true; //changing XP below fires HandleXPChanged again - this ignores it
	
	const FGameplayAttribute XPAttr = UCombatAttributeSet::GetXPAttribute();
	const FGameplayAttribute LevelAttr = UCombatAttributeSet::GetCharacterLevelAttribute();
	const FGameplayAttribute PointsAttr = UCombatAttributeSet::GetStatPointsAttribute();
	
	float XP = ASC->GetNumericAttributeBase(XPAttr);
	float Cost = GetXPCostForNextLevel();
	bool bLeveledUp = false;
	
	//Keep leveling while there's enough XP (a big kill can give more than one level)
	while (Cost > 0.0f && XP >= Cost)
	{
		XP -= Cost;
		ASC->SetNumericAttributeBase(LevelAttr, ASC->GetNumericAttributeBase(LevelAttr) + 1.0f);
		ASC->SetNumericAttributeBase(PointsAttr, ASC->GetNumericAttributeBase(PointsAttr) + StatPointsPerLevel);
		Cost = GetXPCostForNextLevel(); //new level = new cost
		bLeveledUp = true;
	}
	
	//Leftover XP carries over
	if (bLeveledUp)
	{
		ASC->SetNumericAttributeBase(XPAttr, XP);
	}
	
	bIsLevelingUp = false;
	
	//If the menu is open, show the new numbers
	if (bLeveledUp && bStatsMenuOpen && StatsMenu)
	{
		StatsMenu->RefreshMenu();
	}
}
#pragma endregion

#pragma region Player Gameplay Effects
void APlayerCharacter::HandleStatsMenuInput(const struct FInputActionValue& InputActionValue)
{
	ToggleStatsMenu();
}

TSubclassOf<UGameplayEffect> APlayerCharacter::GetStatsEffect() const
{
	//Warrior by default, so PIE straight into the open-world map still works
	ECharacterClass ChosenClass = ECharacterClass::Warrior;
	if (const UCombatGameInstance* GI = Cast<UCombatGameInstance>(GetGameInstance()))
	{
		ChosenClass = GI->SelectedClass;
	}

	const TSubclassOf<UGameplayEffect>* ClassEffect = ClassEffects.Find(ChosenClass);
	return ClassEffect ? *ClassEffect : nullptr;
}

bool APlayerCharacter::TrySpendStatPoint(FGameplayAttribute Stat)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		return false;
	}
	
	//Only the three spendable stats
	const bool bIsSpendableStat = Stat == UCombatAttributeSet::GetVitalityAttribute() || Stat == UCombatAttributeSet::GetEnduranceAttribute() || Stat == UCombatAttributeSet::GetMindAttribute();
	if (!bIsSpendableStat)
	{
		return false;
	}
	
	//Need at least 1 point
	const float Points = ASC->GetNumericAttributeBase(UCombatAttributeSet::GetStatPointsAttribute());
	if (Points < 1.0f)
	{
		return false;
	}
	
	//-1 point, +1 stat
	ASC->SetNumericAttributeBase(UCombatAttributeSet::GetStatPointsAttribute(), Points - 1.0f);
	ASC->SetNumericAttributeBase(Stat, ASC->GetNumericAttributeBase(Stat) + 1.0f);
	return true;
}
#pragma endregion

#pragma region Player Locomotion/Input Actions

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Player Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLookInput);
		
		//Player Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMoveInput);

		//Player Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleJumpInput);
		//Make sure exists first
		if (StatsMenuAction)
		{
			EnhancedInputComponent->BindAction(StatsMenuAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleStatsMenuInput);
			UE_LOG(LogTemp,Warning,TEXT("Stats menu action has been binded"))
		}

		UE_LOG(LogTemp, Warning, TEXT("Input Actions Have been binded!"))
		
		//For input binding of the abilties
		for (const TPair<TObjectPtr<UInputAction>, FGameplayTag>& AbilityInput : AbilityInputActions)
		{
			if (!AbilityInput.Key)
			{
				continue;
			}
			EnhancedInputComponent->BindAction(AbilityInput.Key.Get(), ETriggerEvent::Started,   this, &APlayerCharacter::HandleAbilityInputPressed,  AbilityInput.Value);
			EnhancedInputComponent->BindAction(AbilityInput.Key.Get(), ETriggerEvent::Completed, this, &APlayerCharacter::HandleAbilityInputReleased, AbilityInput.Value);
		}
	}
}
void APlayerCharacter::HandleAbilityInputPressed(FGameplayTag InputTag)
{
	if (bStatsMenuOpen)
	{
		return;
	}
	if (CombatAbilitySystemComponent)
	{
		CombatAbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void APlayerCharacter::HandleAbilityInputReleased(FGameplayTag InputTag)
{
	if (CombatAbilitySystemComponent)
	{
		CombatAbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		//Set input mode on spawn pretty much
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::HandleLookInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputAction = InputActionValue.Get<FVector2D>(); //grabbing the input value from the mouse and right stick (Look input)
	
	//Adding rotation using the above vector
	AddControllerYawInput(InputAction.X);
	AddControllerPitchInput(InputAction.Y);
}

void APlayerCharacter::HandleMoveInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputAction = InputActionValue.Get<FVector2D>(); //grabs the X and Y values which represent movement directions
	
	InputAction.Normalize(); //this makes sure every direction movement is the same speed
	
	//This handles all movement and allows the player to move diagnally
	AddMovementInput(GetMoveFWDdirection()* InputAction.Y + GetRightDirection() * InputAction.X);
}

void APlayerCharacter::HandleJumpInput(const struct FInputActionValue& InputActionValue)
{
	Jump(); //We dont need a "HandleJumpInput" technically we could just call jump, but for consistency we will use it 
}
#pragma endregion Player Locomotion/Input Actions

#pragma region Getters

//----------------------------------------------------------------------------------//
//									Locomotion Getters
//----------------------------------------------------------------------------------//

FVector APlayerCharacter::GetRightDirection() const
{
	return CameraComponent->GetRightVector();
}

FVector APlayerCharacter::GetLookFWDVector() const
{
	return CameraComponent->GetForwardVector();
}

FVector APlayerCharacter::GetMoveFWDdirection() const
{
	return FVector::CrossProduct(GetRightDirection(), FVector::UpVector);
}
#pragma endregion Getters
