// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatAnimInstance.generated.h"

UCLASS()
class UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	//----------------------------------------------------------------------------------//
	//                   Locomotion values (read in the AnimGraph)
	//----------------------------------------------------------------------------------//
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion") float GroundSpeed = 0.0f; //horizontal speed - drives the blend space
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion") float Direction = 0.0f; //-180 to 180, for strafe blend spaces
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion") float VerticalVelocity = 0.0f; //+ going up, - falling
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion") bool bIsInAir = false;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion") bool bIsMoving = false;  //has input AND speed - good for idle <-> move transitions

private:
	UPROPERTY() TObjectPtr<ACharacter> OwningCharacter;
	UPROPERTY() TObjectPtr<class UCharacterMovementComponent> MovementComponent;
};
