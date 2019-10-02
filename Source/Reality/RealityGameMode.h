// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RealityGameMode.generated.h"

UCLASS(minimalapi)
class ARealityGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ARealityGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Minutes;

	FTimerHandle PlayerTime;

	void PlayerTimer();
};



