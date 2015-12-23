// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 *
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	// Set default values for constructor
	ABatteryPickup();
	
	// Override the WasCollected function - use Implementation because it's a Blueprint Native event
	void WasCollected_Implementation() override;
	
	/** Accessor for battery power */
	float GetPower();

protected:
	/** Set the amount fo power the battery gives to the character */
	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = "Power", Meta = (BlueoprintProtected))
		float BatteryPower;
};
