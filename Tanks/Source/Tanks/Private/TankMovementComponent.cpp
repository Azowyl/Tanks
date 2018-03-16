// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


void UTankMovementComponent::Initialise(UTankTracks* TankTrack)
{
	this->TankTracks = TankTrack;
	TArray<UParticleSystem*> ParticleSystems;
	GetOwner()->GetComponents<UParticleSystem>(ParticleSystems);

	if (ParticleSystems.Num == (int32)2)
	{
		// Right and left particle systems 
		RightDust = ParticleSystems[0];
		LeftDust = ParticleSystems[1];
	}
}

void UTankMovementComponent::MoveToMousePosition()
{
	FHitResult hitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);
	Destination = hitResult.Location;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Destination.ToString())
	DirectionWhereToGo =  (Destination - GetOwner()->GetRootComponent()->GetComponentLocation()).GetSafeNormal();
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (!Destination.Equals(NO_DESTINATION))
	{
		auto TankRoot = GetOwner()->GetRootComponent();

		auto DeltaRotator = TankRoot->GetForwardVector().Rotation() - DirectionWhereToGo.Rotation();
		bool DirectionReached = FMath::IsNearlyEqual(FMath::Abs(DeltaRotator.Yaw), 0, 0.5f);

		FVector TankLocation = TankRoot->GetComponentLocation();
		bool DestinationReached = FVector::Dist2D(Destination, TankLocation) < DestinationTolerance;
		
		if (!DestinationReached)
		{
			Cast<USceneComponent>(RightDust)->SetVisibility(true);
			Cast<USceneComponent>(LeftDust)->SetVisibility(true);

			UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist2D(Destination, TankLocation))
			TankTracks->DriveTrack();
		}
		else
		{
			Destination = NO_DESTINATION;

			Cast<USceneComponent>(RightDust)->SetVisibility(false);
			Cast<USceneComponent>(LeftDust)->SetVisibility(false);
		}

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
}


