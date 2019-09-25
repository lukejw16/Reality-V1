// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyProjectile.h"
#include "Path.h"
#include "MyActor.generated.h"

UCLASS()
class REALITY_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FireLocation;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyProjectile> ProjectileClass;

	FTimerHandle Timer;

	FTimerHandle MoveTimer;


	UPROPERTY(EditAnywhere)
		float Delay;

	void Spawn();

	
	void move();

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		TArray<FVector> NewLocation;

	FVector OrginalLocation;
	
	int index;

	UPROPERTY(BlueprintReadWrite)
	int indexMax;

	bool increment;
	int switchonint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APath* Path;

	void RandomPathPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandom;
	
	void SelectMovement();
};
