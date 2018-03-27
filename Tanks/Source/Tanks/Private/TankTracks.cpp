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
	OnComponentHit.AddDynamic(this, &UTankTracks::OnHit);
}

void UTankTracks::DriveTrack()
{
	CurrentThrottle = 1;
}

void UTankTracks::Turn(Direction Direction)
{
	auto RootTank = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	auto AzimuthChange = DegreesPerSecond * GetWorld()->GetDeltaSeconds();
	auto YawNewRotation = RelativeRotation.Yaw + AzimuthChange;

	if (Direction == Direction::Left)
	{
		YawNewRotation = YawNewRotation * -1;
	}

	RootTank->AddLocalRotation(FRotator(0, YawNewRotation, 0));
}

bool UTankTracks::IsAccelerating() const
{
	return CurrentThrottle != 0;
}

void UTankTracks::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	auto ForceApplied = CalculateForceToApply() * CurrentThrottle;
	ApplyForcesOnTracks(ForceApplied, ForceApplied);
	CurrentThrottle = 0;
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

	auto RootTank = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = (RootTank->GetMass() * CorrectionAcceleration);

	this->AddForce(CorrectionForce);
}

FVector UTankTracks::CalculateForceToApply()
{
	return GetForwardVector() * MaxForce * GetWorld()->GetDeltaSeconds();
}

