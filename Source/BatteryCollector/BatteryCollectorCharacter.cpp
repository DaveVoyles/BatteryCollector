// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorCharacter.h"
#include "Pickup.h"
#include "BatteryPickup.h"

//////////////////////////////////////////////////////////////////////////
// ABatteryCollectorCharacter

ABatteryCollectorCharacter::ABatteryCollectorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	// Create collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);

	// Set a base power level for the character
	InitialPower   = 1500.0f;
	CharacterPower = InitialPower;

	// Set the dependence of the speed on power level
	SpeedFactor = 3.0f;
	BaseSpeed   = 20.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABatteryCollectorCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	InputComponent->BindAction("Collect", IE_Pressed, this, &ABatteryCollectorCharacter::CollectPickups);

	InputComponent->BindAxis("MoveForward", this, &ABatteryCollectorCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABatteryCollectorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ABatteryCollectorCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ABatteryCollectorCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ABatteryCollectorCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ABatteryCollectorCharacter::TouchStopped);
}


void ABatteryCollectorCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ABatteryCollectorCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ABatteryCollectorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryCollectorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryCollectorCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABatteryCollectorCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABatteryCollectorCharacter::CollectPickups()
{
	// Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// Keep track of the collected battery power
	float CollectedPower = 0;

	// For each actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		// Cast the actor to a APickup
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
		// If the cast is successful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			// Call the pickup's WasCollected() function
			TestPickup->WasCollected();

			// Check to see if the pickup is also a battery
			auto* const TestBattery = Cast<ABatteryPickup>(TestPickup);
			if (TestBattery) 
			{
				// Incrase the collected power
				CollectedPower += TestBattery->GetPower();
			}
			// Deactivate the pickup
			TestPickup->SetActive(false);
		}
	}
	// If we've collected a battery.....
	if (CollectedPower > 0)
	{
		UpdatePower(CollectedPower);
	}
}

float ABatteryCollectorCharacter::GetInitialPower() 
{
	return InitialPower;
}

float ABatteryCollectorCharacter::GetCurrentPower()
{
	return CharacterPower;
}

// Calleed whenever power is increased or decreased
void ABatteryCollectorCharacter::UpdatePower(float PowerChange)
{
	// Change power
	CharacterPower = CharacterPower + PowerChange;
	// Change speed based on power
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * CharacterPower;
	// Call visual effect
	PowerChangeEffect();
}
