// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_MeleeTrace.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Melee Trace"))
class UANS_MeleeTrace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	//Per-attack hitbox size. -1 = use the component's DefaultRadius
	UPROPERTY(EditAnywhere, Category = "Melee") float RadiusOverride = -1.0f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
