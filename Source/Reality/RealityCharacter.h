// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "RealityCharacter.generated.h"

class UInputComponent;
class AKillBox;

UENUM(BlueprintType)
enum Weapons
{
	Pistol     UMETA(DisplayName = "Pistol"),
	SMG      UMETA(DisplayName = "SMG"),
	Shotgun   UMETA(DisplayName = "Shotgun"),
};


UCLASS(config=Game)


class ARealityCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;


		/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun_SMG;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun_Shotgun;
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation_S_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation_S_1;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	ARealityCharacter();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ARealityProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Timer;

	bool isOnFloor;

	void Shoot();

	bool bSafeSpot;

	bool inAir;

	bool bHasKey;

	bool bCanShoot;

	float WalkSpeed;

	float VelocityFloat;

	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnergyMeter;

	FVector SpawnLocation;

	int TimerCounter;

	bool bCanMove;

	FTimerHandle CanMoveReset;

	void CanMove();

	float Damage;

	FTimerHandle ButtonReset;

	FTimerHandle CanShootHandle;

	FTimerHandle SpeedTimer;

	FTimerHandle HandleTick;

	FTimerHandle Handler;

	FTimerHandle EnergyHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
	FName Level = "FirstpersonExampleMap";

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		//AKillBox* Kill;


	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		UBoxComponent* BoxCollision;

	void DepleteEnergyMeter();

	bool OverlappedWithRamp;

	void DirectionImpulse();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDelay = 0.5f;

	float RightValue;

	float ForwardValue;

	bool Forward;

	FTimerHandle ShootTimer;

	bool Right;

	bool Left;

	bool Back;

	bool AllowedImpulse;

	void ImpulseAllowed();

	void SpeedValue();

	void SetCanShootTrue();

	//weapon variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TEnumAsByte<Weapons> WeaponType;
	int WeaponN;
	

protected:
	
	/** Fires a projectile. */
	void OnFire();

	void OnFireReleased();

	void Impulse();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	
	void TimerReset();

	

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void BoolSwitch();
	virtual void Landed(const FHitResult& Hit) override;

	//weapons variables 
	void WeaponSelection();

};

