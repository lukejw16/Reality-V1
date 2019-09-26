// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RealityCharacter.h"
#include "RealityProjectile.h"
#include "RealitySurf.generated.h"

UCLASS()
class REALITY_API ARealitySurf : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealitySurf();

	FTimerHandle Handler;
	FTimerHandle JumpTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		UBoxComponent* BoxCollision;
	UPROPERTY()
		ARealityCharacter* Character;

	UPROPERTY()
		ARealityProjectile* Projectile;

	UPROPERTY()
		bool bIsForward;

	float PlayerTime;
	
	void ResetHandle();

	void StartGame();

	void CheckEndOverlap();

	void PlayerTimerIncrease();
};
