// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class REALITY_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public: 
	UMySaveGame();

	UPROPERTY(EditAnywhere)
		int Seconds;

	UPROPERTY(EditAnywhere)
		int Minutes;
	
	
	
};
