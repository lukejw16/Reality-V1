// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI_Controller.h"
#include "AICharacter.generated.h"

UCLASS()
class REALITY_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWayPoint* NextWaypoint;

	void FireWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AEnemyProjectile> ProjectileClass;

	
	FVector GunOffset;

	float health = 1.0f;

	void CheckEnemyHealth();
};
