// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RealityCharacter.h"
#include "ExpandingProjectile.generated.h"

UCLASS()
class REALITY_API AExpandingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExpandingProjectile();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	FVector Scale;
	FVector OriginalScale;

	float timecounter;

	FTimerHandle Timer;

	FTimerHandle AttackTimer;

	ARealityCharacter* Character;

	int radius = 30;

	bool bSrink;

	void ScaleIncrease();

	void DepleteEnergy();

	UPROPERTY(EditAnywhere)
		bool Environmental;


};
