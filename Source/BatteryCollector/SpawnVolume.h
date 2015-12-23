// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Returns the WhereToSpawn subobject
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	// BlueprintPure means it will not have execution pins in BP
	UFUNCTION(BlueprintPure, Category = "Spawning")
    FVector GetRandomPointsInVolume();

	/** This function toggles whether or not the spawn volume should spawn pickups */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool bShouldSpawn);


protected:
	// The pickup to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;

private:
	// Box component to specify where pickups should be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta =(AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;	

	// Handle spawning of a new pickup
	void SpawnPickup();

	// The current spawn delay
	float SpawnDelay;

	TArray<class ASpawnVolume*> SpawnVolumeActors;
};
