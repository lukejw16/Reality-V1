// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RealityCharacter.h"
#include "RealityKey.h"
#include "RealityDoor.generated.h"



UCLASS()
class REALITY_API ARealityDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealityDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		UBoxComponent* BoxCollision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class ARealityProjectile* Projectile;

	UPROPERTY()
		ARealityCharacter* Character;

	UPROPERTY(VisibleAnywhere)
		UParticleSystem* PSC;

	FVector* Location;

	void SpawnEmitter();

	void ChangeEnergyMeter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		ARealityKey* Key;

	
		bool bGotKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	    bool bKeyNeeded;
};
