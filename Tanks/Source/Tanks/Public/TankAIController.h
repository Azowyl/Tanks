// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	UPROPERTY(EditDefaultsonly, Category = "Setup")
	float AcceptanceRadius = 10000;
};
