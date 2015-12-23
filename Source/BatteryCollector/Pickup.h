// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Accessor function for returning mesh for pickup
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	/**
	 * Can modify things, but probably shouldn't - pops it off stack, but gets called twice. 
	 * PURE Functions promise not to modify state or the members of the class in any way.
	 * GOOD FOR: Fast calcs that frequently change */
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	/**
	 * Only get run when input execution pin is hit. Can be used for non-modified code.
	 * GOOD FOR: Expensive calcs that aren't expected to change frequently */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	// Function to call when a pickup is collected
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation(); 

protected:
	// True when pickup is used and false when pickup is decactivated
	bool bIsActive;

private:
	// mesh to represent pickup in level
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
	
};
