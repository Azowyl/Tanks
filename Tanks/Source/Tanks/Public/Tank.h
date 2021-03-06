// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Tank.generated.h"

UCLASS()
class TANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void MoveToInputPosition(bool IsForward);

	UFUNCTION(BlueprintCallable)
	bool IsMoving();

	UFUNCTION(BlueprintCallable)
	bool IsAccelerating() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthAsPercentage() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void OnDeath();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint = nullptr;

	UTankBarrel* Barrel = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UTankMovementComponent* MovementComponent = nullptr;

	UPROPERTY(EditDefaultsonly, Category = Setup)
	float ShootCooldown = 3; // seconds

	float LastShootTime;

	UPROPERTY(EditDefaultsonly, Category = Setup)
	USoundBase* ShootingExplosion;

	UPROPERTY(EditDefaultsonly, Category = Setup)
	float InitialHealth = 100;

	float CurrentHealth = InitialHealth;
};
