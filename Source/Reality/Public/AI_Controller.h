// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyProjectile.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class REALITY_API AAI_Controller : public AAIController
{
	GENERATED_BODY()

public:
		AAI_Controller();

		virtual void BeginPlay() override;

		virtual void Possess(APawn* Pawn) override;

		virtual void Tick(float DeltaSeconds) override;

		virtual FRotator GetControlRotation() const override;

		UFUNCTION()
			void OnPawnDetected(TArray<AActor*> DetectedPawn);

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			float AISightRadius = 1200.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			float AISightAge = 5.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			float AILoseSightRadius = AISightRadius + 50.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			float AIFieldOfView = 90.0f;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			class UAISenseConfig_Sight* SightConfig;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bPlayerIsDetected = false;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
			float DistanceToPlayer = 0.0f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
			float WeaponDelay = 0.2;
			

		void Shoot();

		

		FTimerHandle Timer;
	
};
