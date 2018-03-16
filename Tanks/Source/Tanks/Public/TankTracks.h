// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTracks.generated.h"

UENUM()
enum class Direction 
{
	Right ,
	Left
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UTankTracks : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankTracks();

	void DriveTrack();

	void Turn(Direction Direction);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsonly, Category = "Setup")
	float MaxForce = 400000000;

	// Degrees per second used when turn
	UPROPERTY(EditDefaultsonly, Category = "Setup")
	float DegreesPerSecond = 100.0f;

	void ApplyForcesOnTracks(FVector RightTrackForce, FVector LeftTrackForce);

	void AddSideWaysForce();

	FVector CalculateForceToApply();
};
