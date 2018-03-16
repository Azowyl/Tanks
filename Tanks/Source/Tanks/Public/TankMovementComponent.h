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
	UFUNCTION(BlueprintCallable)
	void Initialise(UTankTracks* TankTrack);

	// Moves to a position in 3d world space
	UFUNCTION(BlueprintCallable)
	void MoveToMousePosition();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector Destination = NO_DESTINATION;
	FVector DirectionWhereToGo = FVector::ZeroVector;

	// when the distance between current location and destination is less than this variable, destination is concidered reached
	UPROPERTY(EditDefaultsonly)
	float DestinationTolerance = 0.5f;

	UTankTracks * TankTracks;

	UParticleSystem* Dust;
};
