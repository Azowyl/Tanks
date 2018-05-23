// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	Barrel = FindComponentByClass<UTankBarrel>();
	
	if (MovementComponent) {
		MovementComponent->Initialise(FindComponentByClass<UTankTracks>());
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Tank Movement"));
}

void ATank::Fire()
{
	if ((GetWorld()->GetTimeSeconds() - LastShootTime) < ShootCooldown) { return; }

	auto Location = Barrel->GetSocketLocation(FName("Projectile"));
	auto Rotation = Barrel->GetSocketRotation(FName("Projectile"));

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Location, Rotation);
	if (Projectile)
	{
		Projectile->LaunchProjectile();
		UGameplayStatics::PlaySoundAtLocation(this, ShootingExplosion, GetActorLocation());
	}

	LastShootTime = GetWorld()->GetTimeSeconds();
}

void ATank::MoveToInputPosition(bool IsForward)
{
	if (MovementComponent) {
		MovementComponent->MoveToMousePosition(IsForward);
	}
}

bool ATank::IsMoving()
{
	if (!MovementComponent) { return false; }

	return MovementComponent->IsMoving();
}

bool ATank::IsAccelerating() const
{
	return MovementComponent->IsAccelerating();
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return 0.0f;
}
