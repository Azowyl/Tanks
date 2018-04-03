// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


void UTankMovementComponent::Initialise(UTankTracks* TankTrack)
{
	this->TankTracks = TankTrack;
}

void UTankMovementComponent::MoveToMousePosition()
{
	FHitResult hitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);
	Destination = hitResult.Location;
	FacingDirection = (Destination - GetOwner()->GetRootComponent()->GetComponentLocation()).GetSafeNormal();
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Destination.Equals(NO_DESTINATION))
	{
		Move();
		LookInDirection(FacingDirection);
	}
}

void UTankMovementComponent::Move()
{
	auto TankRoot = GetOwner()->GetRootComponent();

	FVector TankLocation = TankRoot->GetComponentLocation();
	bool DestinationReached = FVector::Dist2D(Destination, TankLocation) < DestinationTolerance;

	if (!DestinationReached)
	{
		TankTracks->DriveTrack();
	}
	else
	{
		Destination = NO_DESTINATION;
	}
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	Destination = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FacingDirection = (Destination - GetOwner()->GetRootComponent()->GetComponentLocation()).GetSafeNormal();
}

bool UTankMovementComponent::IsMoving() const
{
	auto Velocity2d = GetOwner()->GetRootComponent()->GetComponentVelocity();
	Velocity2d.Z = 0;
	return !Velocity2d.IsNearlyZero(MoveTolerance);
}

bool UTankMovementComponent::IsCloseToDestination() const
{
	return FVector::Dist2D(Destination, GetOwner()->GetRootComponent()->GetComponentLocation()) < CloseToDestinationTolerance;
}

void UTankMovementComponent::LookInDirection(FVector Direction)
{
	auto DeltaRotator = GetOwner()->GetRootComponent()->GetForwardVector().Rotation() - Direction.Rotation();
	bool DirectionReached = FMath::IsNearlyEqual(FMath::Abs(DeltaRotator.Yaw), 0, DirectionReachedTolerance);

	if (!DirectionReached)
	{
		if (DeltaRotator.Yaw < 0)
		{
			TankTracks->Turn(Direction::Right);
		}
		else
		{
			TankTracks->Turn(Direction::Left);
		}
	}
}
