// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RealityCharacter.h"
#include "Components/BoxComponent.h"
#include "SplineManager.generated.h"

UCLASS()
class REALITY_API ASplineManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddToSpline();

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = YourCategory)
		USplineComponent* YourSplineComponent;

	int SplineLegnth;
	FVector Points;
	FTimerHandle Handler;
	float Multiple;
	float Distance;
	int Counter;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SphereMesh;

	UPROPERTY()
		ARealityCharacter* Character;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};
