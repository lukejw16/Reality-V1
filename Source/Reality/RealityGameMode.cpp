// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RealityGameMode.h"
#include "RealityHUD.h"
#include "RealityCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARealityGameMode::ARealityGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARealityHUD::StaticClass();

	
}

void ARealityGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(PlayerTime, this, &ARealityGameMode::PlayerTimer, 1.0f, true);
}

void ARealityGameMode::PlayerTimer()
{
	if (Seconds < 59)
	{
		Seconds++;
	}
	if (Seconds == 59)
	{
		Seconds = 0;
		Minutes += 1;
	}
}
