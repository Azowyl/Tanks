// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


void UTankMovementComponent::Initialise(UTankTracks* TankTrack)
{
	this->TankTracks = TankTrack;
}

void UTankMovementComponent::MoveToMousePosition(bool MoveForward)
{
	FHitResult hitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);
	Destination = hitResult.Location;
	DirectionToFace = (Destination - GetOwner()->GetRootComponent()->GetComponentLocation()).GetSafeNormal();

	bMustMoveForward = MoveForward;
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Destination.Equals(NO_DESTINATION))
	{
		Move();
	}

	if (!DirectionToFace.Equals(FVector::ZeroVector))
	{
		if (!bMustMoveForward)
		{
			LookInDirection(-1 * DirectionToFace);
		}
		else
		{
			LookInDirection(DirectionToFace);
		}
	}
}

void UTankMovementComponent::Move()
{
	auto TankRoot = GetOwner()->GetRootComponent();

	FVector TankLocation = TankRoot->GetComponentLocation();
	FVector DistanceToDestination = Destination - TankLocation;
	
	bool DestinationReached = (FMath::Abs(DistanceToDestination.X) < DestinationTolerance) || (FMath::Abs(DistanceToDestination.Y) < DestinationTolerance);

	if (!DestinationReached)
	{
		TankTracks->DriveTrack(bMustMoveForward);
	}
	else
	{
		Destination = NO_DESTINATION;
	}
}

bool UTankMovementComponent::IsMoving() const
{
	auto Velocity2d = GetOwner()->GetRootComponent()->GetComponentVelocity();
	Velocity2d.Z = 0;
	return !Velocity2d.IsNearlyZero(MoveTolerance);
}

void UTankMovementComponent::LookInDirection(FVector Direction)
{
	auto FromRotation = GetOwner()->GetRootComponent()->GetForwardVector().Rotation().Yaw;
	auto ToRotation = Direction.Rotation().Yaw;

	// this two if statement, make tank turn in shortest direction
	if (FromRotation < 0 && FromRotation < (ToRotation - 180))
	{
		FromRotation += 360;
	}

	if (ToRotation < 0 && ToRotation < (FromRotation - 180)) 
	{
		ToRotation += 360;
	}
	//
	auto DeltaRotator = ToRotation - FromRotation;
	bool DirectionReached = FMath::IsNearlyEqual(DeltaRotator, 0, DirectionReachedTolerance);

	if (!DirectionReached)
	{
		if (DeltaRotator > 0)
		{
			TankTracks->Turn(Direction::Right);
		}
		else
		{
			TankTracks->Turn(Direction::Left);
		}
	}
}

bool UTankMovementComponent::IsAccelerating() const
{
	return TankTracks->IsAccelerating();
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) {
	Destination = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation();
}
