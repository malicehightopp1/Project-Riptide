// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeHitBoxComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMeleeHitBoxComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UMeleeHitBoxComponent();

	//The mesh the sockets are on (character mesh for fists, weapon mesh later)
	void SetTraceSource(USceneComponent* InSource) { TraceSource = InSource; }

	//Called by the ANS_MeleeTrace notify in attack montages
	void BeginTrace(float RadiusOverride = -1.0f);
	void EndTrace();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Melee") FName StartSocket = TEXT("TraceStart");
	UPROPERTY(EditAnywhere, Category = "Melee") FName EndSocket = TEXT("TraceEnd");
	UPROPERTY(EditAnywhere, Category = "Melee") float DefaultRadius = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Melee", meta = (ClampMin = 2)) int32 PointsAlongBlade = 4;
	UPROPERTY(EditAnywhere, Category = "Melee|Debug") bool bDrawDebug = true;

private:
	void SampleBladePoints(TArray<FVector>& Out) const;
	void HandleHit(const FHitResult& Hit);

	UPROPERTY() TObjectPtr<USceneComponent> TraceSource;

	TArray<FVector> PreviousPoints;           //where the blade was last frame
	TSet<TWeakObjectPtr<AActor>> HitThisSwing; //so one swing only hits each enemy once
	float ActiveRadius = 0.0f;
	bool bTracing = false;
		
};
