// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RealityCharacter.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class REALITY_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* Mesh;
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY()
		ARealityCharacter* Character;

	float Damage;
};
