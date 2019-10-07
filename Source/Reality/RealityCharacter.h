// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
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

	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun_SMG;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun_Shotgun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation_S_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation_S_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		UBoxComponent* BoxCollision;
	

public:
	ARealityCharacter();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay();

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ARealityProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* GunShot;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* LandSound;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnergyMeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FJumpMeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
		FName Level = "FirstpersonExampleMap";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WeaponDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TEnumAsByte<Weapons> WeaponType;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<UCameraShake> MyShake;

	void SaveGame();

	void LoadGame();

	int loopint;

	bool isOnFloor;

	void Shoot();

	void JumpMeter();

	bool bSafeSpot;

	bool inAir;

	bool bHasKey;

	bool bCanShoot;
	
	float WalkSpeed;

	float VelocityFloat;

	FVector Velocity;
	
	UPROPERTY(BlueprintReadWrite)
	FVector SpawnLocation;
	UPROPERTY(BlueprintReadWrite)
	FRotator SpawnRotation;

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

	FTimerHandle JumpMeterHandle;
		
	void DepleteEnergyMeter();

	bool OverlappedWithRamp;

	void DirectionImpulse();

	float RightValue;

	float ForwardValue;

	bool Forward;

	FTimerHandle ShootTimer;

	FTimerHandle FOVTimer;

	bool Right;

	bool Left;

	bool Back;

	bool AllowedImpulse;

	void ImpulseAllowed();

	void SpeedValue();

	void SetCanShootTrue();
	
	int WeaponN;
	
	void ChangeFieldofView();

	bool Reverse;

	bool stop;

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
	
	
	TouchData	TouchItem;

	
	void TimerReset();

	

	

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

	void CheckPlayerHealth();

	void Respawn();

	float Radius;

	void StartGame();

	void changeRadius();

	UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	UDecalComponent* Decal;

	FTimerHandle HandleTemp;

	FLinearColor Color;

	float lerp;
	float Afloat;

	UPROPERTY(BlueprintReadWrite)
	bool bMainMenu;

	UPROPERTY(BlueprintReadWrite)
	bool bCheckpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<class UUserWidget> wMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<class UUserWidget> wPauseMenu;

	UUserWidget* MyMainMenu;

	UUserWidget* MyPauseMenu;

	void ShowScore();

	void PauseMenu();
};

