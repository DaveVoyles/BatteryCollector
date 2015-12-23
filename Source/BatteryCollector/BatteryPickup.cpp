// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "BatteryPickup.h"

// Set default values
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	// Set base power level of battery
	BatteryPower = 150.0f;
	UE_LOG(LogTemp, Warning, TEXT("I Am alive!"));
}


void ABatteryPickup::WasCollected_Implementation()
{
	// Use base pickup behavior
	Super::WasCollected_Implementation();
	Destroy();
}

// Report power of battery
float ABatteryPickup::GetPower() 
{
	return BatteryPower;
}