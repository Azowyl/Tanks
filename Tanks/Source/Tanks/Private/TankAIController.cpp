// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	TankMovementComponent = GetPawn()->FindComponentByClass<UTankMovementComponent>();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerTank) { return; }
	
	if (TankMovementComponent)
	{
		auto FacingDirection = (PlayerTank->GetTargetLocation() - GetPawn()->GetTargetLocation()).GetSafeNormal();
		TankMovementComponent->LookInDirection(FacingDirection);
	}

	MoveToActor(PlayerTank, AcceptanceRadius);
	Cast<ATank>(GetPawn())->Fire();
}
