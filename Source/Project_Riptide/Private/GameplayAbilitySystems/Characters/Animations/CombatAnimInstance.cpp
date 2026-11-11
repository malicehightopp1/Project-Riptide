// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Animations/CombatAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		MovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UCombatAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Character can be missing on the first frames (and in the editor preview) - try again
	if (!OwningCharacter || !MovementComponent)
	{
		NativeInitializeAnimation();
		if (!OwningCharacter || !MovementComponent)
		{
			return;
		}
	}

	const FVector Velocity = MovementComponent->Velocity;

	GroundSpeed = Velocity.Size2D();
	VerticalVelocity = Velocity.Z;
	bIsInAir = MovementComponent->IsFalling();
	bIsMoving = GroundSpeed > 3.0f && !MovementComponent->GetCurrentAcceleration().IsNearlyZero();

	//Direction of movement relative to where the character faces (0 = forward, 90 = right, -90 = left)
	const FVector LocalVelocity = OwningCharacter->GetActorRotation().UnrotateVector(Velocity);
	Direction = GroundSpeed > 3.0f ? FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X)) : 0.0f;
}