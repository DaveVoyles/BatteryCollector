// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "BatteryCollectorGameMode.generated.h"

// enum to store the current state of gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState
{
	EPlaying,
	EGameOver,
	Ewon,
	EUnknown
};

// Enum to adjust store the current difficulty multiplier
//UENUM(BlueprintType)
//enum class EPowerToWinDifficulty
//{
//	EEASY,
//	ENORMAL,
//	EDIFFICULT
//};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

	/** Returns power needed to win - needed for the HUD */
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	/** Returns the current playing state */
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	/** Sets a new playing state */
	void SetCurrentState(EBatteryPlayState NewState);

	/** Returns Difficulty Multiplier */
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetDifficultyMultiplier() const;

	/** Returns PowerToWinMultiplier */
	//UFUNCTION(BlueprintPure, Category = "Power")
	//EPowerToWinDifficulty GetPowerToWinDifficulty() const;

	///** Sets the PowerToWinDiffculty */
	//void SetPowerToWinDifficulty(EPowerToWinDifficulty Difficulty);

protected:
	/**The rate at which the character lsoes power */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "True"))
	float DecayRate;

	/** Power needed to win the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "True"))
    float PowerToWin;

	/** The Widget class to use for our HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "True"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "True"))
	float DifficultyMultiplier;


	//UPROPERTY(BluePrintReadWrite, Category = "Difficulty")
	//void SetCurrentDifficultyMultiplier(EPowerToWinDifficulty Difficulty);


private:
	/** Keeps track of the current playing state */
	EBatteryPlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	/** Handle any function calls thast rely upon changing the state of our game */
	void HandleCurrentState(EBatteryPlayState NewState);

	/** Keeps track of current PowerToWinDifficulty */
	//EPowerToWinDifficulty CurrPowerToWinDifficulty;

	//void HandleDifficulty(EPowerToWinDifficulty Difficulty);

};



