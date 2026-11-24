// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Components/MeleeHitBoxComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySystems/Tags/CombatGameplayTags.h"

UMeleeHitBoxComponent::UMeleeHitBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false; //only ticks while a swing is active
}

void UMeleeHitBoxComponent::BeginTrace(float RadiusOverride)
{
	if (!TraceSource)
	{
		UE_LOG(LogTemp, Error, TEXT("[HITBOX] %s has no TraceSource"), *GetNameSafe(GetOwner()));
		return;
	}

	ActiveRadius = RadiusOverride > 0.0f ? RadiusOverride : DefaultRadius;
	HitThisSwing.Reset();
	SampleBladePoints(PreviousPoints);
	bTracing = true;
	SetComponentTickEnabled(true);
}

void UMeleeHitBoxComponent::EndTrace()
{
	bTracing = false;
	SetComponentTickEnabled(false);
	PreviousPoints.Reset();
}

void UMeleeHitBoxComponent::SampleBladePoints(TArray<FVector>& Out) const
{
	Out.Reset();
	const FVector Start = TraceSource->GetSocketLocation(StartSocket);
	const FVector End = TraceSource->GetSocketLocation(EndSocket);

	for (int32 i = 0; i < PointsAlongBlade; ++i)
	{
		const float Alpha = static_cast<float>(i) / (PointsAlongBlade - 1);
		Out.Add(FMath::Lerp(Start, End, Alpha));
	}
}

void UMeleeHitBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bTracing || !TraceSource)
	{
		return;
	}

	TArray<FVector> CurrentPoints;
	SampleBladePoints(CurrentPoints);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(MeleeTrace), false, GetOwner()); //ignore ourselves
	const FCollisionObjectQueryParams ObjectParams(ECC_Pawn); //only look for characters
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(ActiveRadius);

	//Sweep each point from where it WAS to where it IS - fast swings can't skip through enemies
	for (int32 i = 0; i < CurrentPoints.Num() && i < PreviousPoints.Num(); ++i)
	{
		TArray<FHitResult> Hits;
		GetWorld()->SweepMultiByObjectType(Hits, PreviousPoints[i], CurrentPoints[i], FQuat::Identity, ObjectParams, Sphere, Params);

		if (bDrawDebug)
		{
			DrawDebugLine(GetWorld(), PreviousPoints[i], CurrentPoints[i], FColor::Yellow, false, 1.0f);
			DrawDebugSphere(GetWorld(), CurrentPoints[i], ActiveRadius, 8, Hits.Num() > 0 ? FColor::Red : FColor::Green, false, 1.0f);
		}

		for (const FHitResult& Hit : Hits)
		{
			HandleHit(Hit);
		}
	}

	PreviousPoints = MoveTemp(CurrentPoints);
}

void UMeleeHitBoxComponent::HandleHit(const FHitResult& Hit)
{
	AActor* HitActor = Hit.GetActor();
	if (!HitActor || HitActor == GetOwner() || HitThisSwing.Contains(HitActor))
	{
		return;
	}
	HitThisSwing.Add(HitActor);

	UE_LOG(LogTemp, Warning, TEXT("[HITBOX] %s hit %s"), *GetNameSafe(GetOwner()), *GetNameSafe(HitActor)); //TEMP

	//Tell our own ASC "I hit something" - the attack ability is listening for this
	FGameplayEventData Payload;
	Payload.EventTag = CombatTags::Event_Melee_Hit;
	Payload.Instigator = GetOwner();
	Payload.Target = HitActor;
	Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatTags::Event_Melee_Hit, Payload);
}