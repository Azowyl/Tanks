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
	DirectionWhereToGo =  (Destination - GetOwner()->GetRootComponent()->GetComponentLocation()).GetSafeNormal();
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (!Destination.Equals(NO_DESTINATION))
	{
		auto TankRoot = GetOwner()->GetRootComponent();
		auto DeltaRotator = TankRoot->GetForwardVector().Rotation() - DirectionWhereToGo.Rotation();
		UE_LOG(LogTemp, Warning, TEXT("%f"), DeltaRotator.Yaw)
		bool DirectionReached = FMath::IsNearlyEqual(DeltaRotator.Yaw, 0, 10.0f);
		bool DestinationReached = FVector::Distance(Destination, TankRoot->GetComponentLocation()) < DestinationTolerance;
		
		if (!DestinationReached)
		{
			TankTracks->DriveTrack();
		}
		else
		{
			Destination = NO_DESTINATION;
		}

		if (!DirectionReached)
		{
			if (DeltaRotator.Yaw > 0)
			{
				TankTracks->TurnLeft();
			}
			else
			{
				TankTracks->TurnRight();
			}
			DirectionWhereToGo = (Destination - GetOwner()->GetRootComponent()->GetComponentLocation()).GetSafeNormal();
		}
	}
}


