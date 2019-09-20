// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealityCharacter.h"
#include "RealityProjectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RealityCore.generated.h"

UCLASS()
class REALITY_API ARealityCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealityCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		ARealityCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Health;

	float damageTaken;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		class UBoxComponent* Box;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh;

	UPROPERTY()
		ARealityProjectile* Projectile;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
