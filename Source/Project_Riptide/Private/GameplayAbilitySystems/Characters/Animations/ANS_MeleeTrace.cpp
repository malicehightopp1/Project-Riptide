// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Characters/Animations/ANS_MeleeTrace.h"

#include "GameplayAbilitySystems/Components/MeleeHitBoxComponent.h"

//The montage editor preview has no hitbox - these null checks stop it crashing
static UMeleeHitBoxComponent* FindHitbox(USkeletalMeshComponent* MeshComp)
{
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	return Owner ? Owner->FindComponentByClass<UMeleeHitBoxComponent>() : nullptr;
}

void UANS_MeleeTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UMeleeHitBoxComponent* Hitbox = FindHitbox(MeshComp))
	{
		Hitbox->BeginTrace(RadiusOverride);
	}
}

void UANS_MeleeTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UMeleeHitBoxComponent* Hitbox = FindHitbox(MeshComp))
	{
		Hitbox->EndTrace();
	}
}