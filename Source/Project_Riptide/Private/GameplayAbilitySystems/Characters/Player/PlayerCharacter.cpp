// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilitySystems/Widgets/PlayerHUDWidget.h"

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
void APlayerCharacter::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
	
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
bool APlayerCharacter::TryLevelUpStat(FGameplayAttribute Stat)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC || !CombatAttributeSet)
	{
		return false;
	}

	//Only the three spendable stats
	const bool bIsSpendableStat = Stat == UCombatAttributeSet::GetVitalityAttribute() || Stat == UCombatAttributeSet::GetEnduranceAttribute() || Stat == UCombatAttributeSet::GetMindAttribute();
	if (!bIsSpendableStat)
	{
		return false;
	}

	const float Cost = GetXPCostForNextLevel();
	const float CurrentXP = ASC->GetNumericAttributeBase(UCombatAttributeSet::GetXPAttribute());
	if (CurrentXP < Cost)
	{
		return false;
	}

	const FGameplayAttribute LevelAttr = UCombatAttributeSet::GetCharacterLevelAttribute();

	ASC->SetNumericAttributeBase(UCombatAttributeSet::GetXPAttribute(), CurrentXP - Cost);
	ASC->SetNumericAttributeBase(Stat, ASC->GetNumericAttributeBase(Stat) + 1.0f);
	ASC->SetNumericAttributeBase(LevelAttr, ASC->GetNumericAttributeBase(LevelAttr) + 1.0f);
	return true;
}

float APlayerCharacter::GetXPCostForNextLevel() const
{
	if (!CombatAttributeSet)
	{
		return 0.0f;
	}
	return UCombatAttributeSet::GetXPCostForLevel(CombatAttributeSet->GetCharacterLevel());
}
#pragma endregion

#pragma region Player Gameplay Effects
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
