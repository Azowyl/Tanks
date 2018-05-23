// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankTracks.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "TankMovementComponent.generated.h"

#define NO_DESTINATION FVector::ZeroVector

/**
 * Responsable for tank's movement 
 */
UCLASS(meta = (BlueprintSpawnableComponent), Category = "Movement")
class TANKS_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	void Initialise(UTankTracks* TankTrack);

	void MoveToMousePosition();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsMoving() const;

	void LookInDirection(FVector Direction);

	bool IsAccelerating() const;

private:
	FVector Destination = NO_DESTINATION;
	FVector DirectionToFace = FVector::ZeroVector;

	// when the distance between current location and destination is less than this variable, destination is concidered reached
	UPROPERTY(EditDefaultsonly)
	float DestinationTolerance = 0.5f;

	UTankTracks * TankTracks;

	// the tank is not moving if velocity is +- this value
	UPROPERTY(EditDefaultsonly, Category = "Setup")
	float MoveTolerance = 1;

	UPROPERTY(EditDefaultsonly, Category = "Setup")
	float DirectionReachedTolerance = 1;

	// Moves to Destination
	void Move();

	// Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
};
