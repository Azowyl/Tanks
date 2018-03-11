// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTracks.h"


// Sets default values for this component's properties
UTankTracks::UTankTracks()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UTankTracks::BeginPlay()
{
	Super::BeginPlay();
}

void UTankTracks::DriveTrack()
{
	auto ForceApplied = CalculateForceToApply();
	ApplyForcesOnTracks(ForceApplied, ForceApplied);
}

void UTankTracks::TurnRight()
{
	auto ForceApplied = CalculateForceToApply() * 2;
	ApplyForcesOnTracks(ForceApplied, -ForceApplied);
}

void UTankTracks::TurnLeft()
{
	auto ForceApplied = CalculateForceToApply() * 2;
	ApplyForcesOnTracks(-ForceApplied, ForceApplied);
}

void UTankTracks::ApplyForcesOnTracks(FVector RightTrackForce, FVector LeftTrackForce)
{
	auto RootTank = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	RootTank->AddForceAtLocation(RightTrackForce, GetSocketLocation(FName("RightTrack")));
	RootTank->AddForceAtLocation(LeftTrackForce, GetSocketLocation(FName("LeftTrack")));
	AddSideWaysForce();
}

void UTankTracks::AddSideWaysForce()
{
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	auto CorrectionAcceleration = -SlippageSpeed / GetWorld()->GetDeltaSeconds() * GetRightVector();

	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration);

	this->AddForce(CorrectionForce);
}

FVector UTankTracks::CalculateForceToApply()
{
	return GetForwardVector() * MaxForce * GetWorld()->GetDeltaSeconds();
}

