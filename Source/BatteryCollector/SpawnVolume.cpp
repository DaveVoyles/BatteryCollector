// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create box component to represent the spawn volume
	WhereToSpawn  = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	// Set the spawn delay range
	SpawnDelayRangeLow  = 1.0f;
	SpawnDelayRangeHigh = 3.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FVector ASpawnVolume::GetRandomPointsInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ASpawnVolume::SpawnPickup()
{
	// If we have set something to spawn:
	if (WhatToSpawn != NULL)
	{
		// Check for a valid world:
		UWorld* const World = GetWorld();
		if (World)
		{
			// Set spawn params
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner      = this;
			SpawnParams.Instigator = Instigator;
			
			// Get Location to spawn at
			FVector SpawnLocation = GetRandomPointsInVolume();

			// Get random rotation
			FRotator SpawnRotation;
			SpawnRotation.Yaw   = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll  = FMath::FRand() * 360.0f;

			// Spawn teh pickup
			APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::RandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	// Spawn batteries
	if (bShouldSpawn)
	{
		// Set the timer on spawn pickup
		SpawnDelay = FMath::RandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	}
	else
	{
		// Clear the timer to spawn pickup
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}