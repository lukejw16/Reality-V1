// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RealityCharacter.h"
#include "RealityProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"
#include "MotionControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ARealityCharacter

class UCaspuleComponent;

ARealityCharacter::ARealityCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(GetCapsuleComponent());
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	// SMG
	FP_Gun_SMG = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun_SMG"));
	FP_Gun_SMG->SetupAttachment(RootComponent);
	//Shotgun
	FP_Gun_Shotgun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun_Shotgun"));
	FP_Gun_Shotgun->SetupAttachment(RootComponent);
	FP_Gun_Shotgun->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	FP_MuzzleLocation_S_0 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle_Shotgun_0"));
	FP_MuzzleLocation_S_0->SetupAttachment(FP_Gun_Shotgun);
	FP_MuzzleLocation_S_0->SetRelativeLocation(FVector(50.2f, 148.4f, -10.6f));

	FP_MuzzleLocation_S_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle_Shotgun_1"));
	FP_MuzzleLocation_S_1->SetupAttachment(FP_Gun_Shotgun);
	FP_MuzzleLocation_S_1->SetRelativeLocation(FVector(-50.2f, -148.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
			// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	Timer = 1.5f;
	
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	EnergyMeter = 0.5f;
	isOnFloor = true;
	Right = false;
	Forward = false;
	AllowedImpulse = false;
	bSafeSpot = true;
	bCanShoot = true;
}

void ARealityCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	inAir = GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling;
	
	FString TheFloatString = FString::SanitizeFloat(GetCharacterMovement()->MaxWalkSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatString);
	
}


void ARealityCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	WeaponSelection();

	GetWorld()->GetTimerManager().SetTimer(HandleTick, this, &ARealityCharacter::DepleteEnergyMeter, 0.2f, true);

	GetWorld()->GetTimerManager().SetTimer(SpeedTimer, this, &ARealityCharacter::SpeedValue, 0.5f, true);

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARealityCharacter::OnOverlapEnd);

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

void ARealityCharacter::Shoot()
{
	switch (WeaponType)
	{
	case Pistol:
		if (bCanShoot && bSafeSpot == false && WeaponType==Pistol)
		{
			if (ProjectileClass != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					if (bUsingMotionControllers)
					{
						const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
						const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
					}
					else
					{

						bCanShoot = false;

						const FRotator SpawnRotation = GetControlRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the projectile at the muzzle
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

						// Energy depleted 
						FMath::Clamp(EnergyMeter -= 0.1f, 0.0f, 1.0f);

						GetWorld()->GetTimerManager().SetTimer(CanShootHandle, this, &ARealityCharacter::SetCanShootTrue, WeaponDelay, false);

					}
				}
			}
		}
		
	case SMG:
		if (bSafeSpot == false && WeaponType == SMG)
		{
			if (ProjectileClass != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					if (bUsingMotionControllers)
					{
						const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
						const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
					}
					else
					{

						

						const FRotator SpawnRotation = GetControlRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation());
						
						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the projectile at the muzzle
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

						// Energy depleted 
						FMath::Clamp(EnergyMeter -= 0.05f, 0.0f, 1.0f);

						GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &ARealityCharacter::OnFire, WeaponDelay, true);
						

					}
				}
			}
		}
		
		break;
	case Shotgun:
		if (bCanShoot && bSafeSpot == false && WeaponType == Shotgun)
		{
			if (ProjectileClass != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					if (bUsingMotionControllers)
					{
						const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
						const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
					}
					else
					{

						bCanShoot = false;

						const FRotator SpawnRotation = GetControlRotation();
						
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation());
						const FVector SpawnLocation_0 = ((FP_MuzzleLocation_S_0 != nullptr) ? FP_MuzzleLocation_S_0->GetComponentLocation() : GetActorLocation());
						const FVector SpawnLocation_1 = ((FP_MuzzleLocation_S_1 != nullptr) ? FP_MuzzleLocation_S_1->GetComponentLocation() : GetActorLocation());
						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						
						// spawn the projectile at the muzzle
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation_0, SpawnRotation, ActorSpawnParams);
						World->SpawnActor<ARealityProjectile>(ProjectileClass, SpawnLocation_1, SpawnRotation, ActorSpawnParams);
						// Energy depleted 
						FMath::Clamp(EnergyMeter -= 0.05f, 0.0f, 1.0f);

						
						GetWorld()->GetTimerManager().SetTimer(CanShootHandle, this, &ARealityCharacter::SetCanShootTrue, WeaponDelay, false);

					}
				}
			}
		}

		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARealityCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARealityCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ARealityCharacter::OnFireReleased);

	PlayerInputComponent->BindAction("Impulse", IE_Pressed, this, &ARealityCharacter::Impulse);
	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARealityCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ARealityCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARealityCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARealityCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARealityCharacter::LookUpAtRate);
}

void ARealityCharacter::DepleteEnergyMeter()
{
	if (bSafeSpot == false)
	{
		if (inAir&&EnergyMeter < 1.0f &&OverlappedWithRamp == false)
		{
			FMath::Clamp(EnergyMeter += 0.005f, 0.0f, 1.0f);
		}

		if (inAir&&EnergyMeter < 1.0f &&OverlappedWithRamp == true)
		{
			FMath::Clamp(EnergyMeter += 0.02f, 0.0f, 1.0f);
		}

		if (EnergyMeter >= 0.0f && inAir == false)
		{
			FMath::Clamp(EnergyMeter -= 0.02f, 0.0f, 1.0f);
		}

		if (EnergyMeter <= 0.0f)
		{
			GetWorldTimerManager().ClearTimer(EnergyHandler);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DEAD"));
			SetActorLocation(SpawnLocation);
			EnergyMeter = 1.0f;
		}
	}
	
}

void ARealityCharacter::DirectionImpulse()
{
	const FVector RightDir = this->GetActorRightVector();
	const FVector UpDir = this->GetActorForwardVector();

	if (RightValue == 1.0f && Right && Forward == false)
	{
		
		GetCharacterMovement()->Velocity += RightDir * 1000.0f;
	}

	if (RightValue == -1.0f && Left && Forward == false)
	{
		GetCharacterMovement()->Velocity += RightDir * -1000.0f;
	}
	
	if (ForwardValue == 1.0f && Forward && Right == false)
	{
		GetCharacterMovement()->Velocity += UpDir * 1000.0f;
	}
	
	if (ForwardValue == -1.0f && Back && Right == false)
	{
		GetCharacterMovement()->Velocity += UpDir * -1000.0f;
	}
	

	
}

void ARealityCharacter::ImpulseAllowed()
{
	AllowedImpulse = false;
	GetWorldTimerManager().ClearTimer(ButtonReset);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ImpulseReady"));
}

void ARealityCharacter::SpeedValue()
{
	FString TheFloatStr = FString::SanitizeFloat(GetCharacterMovement()->Velocity.Size());
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
}

void ARealityCharacter::SetCanShootTrue()
{
	bCanShoot = true;
}

void ARealityCharacter::OnFire()
{
	
	Shoot();
}

void ARealityCharacter::OnFireReleased()
{
	GetWorldTimerManager().ClearTimer(ShootTimer);
}

void ARealityCharacter::Impulse()
{

	if (AllowedImpulse == false)
	{
		AllowedImpulse = true;
		GetWorld()->GetTimerManager().SetTimer(ButtonReset, this, &ARealityCharacter::ImpulseAllowed, 1.0f, false);
		DirectionImpulse();

	}
	
}

void ARealityCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARealityCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ARealityCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ARealityCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);
	const FVector disVec = Hit.ImpactPoint;
	isOnFloor = true;

	GetWorld()->GetTimerManager().SetTimer(Handler, this, &ARealityCharacter::BoolSwitch, 0.5f, true);

	
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Landed"));
	}

	
	
}

void ARealityCharacter::WeaponSelection()
{
	switch (WeaponType)
	{
	case Pistol:
		FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		FP_Gun->SetHiddenInGame(false);
		FP_Gun_SMG->SetHiddenInGame(true);
		FP_Gun_Shotgun->SetHiddenInGame(true);
		FP_MuzzleLocation->AddLocalOffset(FVector(0.000004, 0.999992, 10.000000), false, 0, ETeleportType::None);
		FP_MuzzleLocation->AddLocalRotation(FRotator(0.0f, -90.0f, 0.0f));
		FP_MuzzleLocation->AttachToComponent(FP_Gun, FAttachmentTransformRules::SnapToTargetIncludingScale);
		
		
		break;
	case SMG:
		FP_Gun_SMG->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		FP_Gun->SetHiddenInGame(true);
		FP_Gun_SMG->SetHiddenInGame(false);
		FP_Gun_Shotgun->SetHiddenInGame(true);
		FP_MuzzleLocation->AddLocalOffset(FVector(0.000004, 0.999992, 0.000000), false, 0, ETeleportType::None);
		
		FP_MuzzleLocation->AttachToComponent(FP_Gun_SMG, FAttachmentTransformRules::KeepWorldTransform);
		WeaponDelay = 0.3f;
		
		break;
	case Shotgun:
		FP_Gun_Shotgun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		FP_Gun->SetHiddenInGame(true);
		FP_Gun_SMG->SetHiddenInGame(true);
		FP_Gun_Shotgun->SetHiddenInGame(false);
		FP_MuzzleLocation->AddLocalOffset(FVector(0.000004, 0.999992, 0.000000), false, 0, ETeleportType::None);
		
		FP_MuzzleLocation->AttachToComponent(FP_Gun_Shotgun, FAttachmentTransformRules::KeepRelativeTransform);
		
		break;
	default:
		break;
	}
	
}

void ARealityCharacter::TimerReset()
{
	if (isOnFloor)
	{
		
		FMath::Clamp(GetCharacterMovement()->AirControl -= 0.1f, 0.5f, 1.0f);
		FMath::Clamp(GetCharacterMovement()->BrakingFrictionFactor += 0.4f, 0.0f, 2.0f);
		FMath::Clamp(GetCharacterMovement()->GroundFriction += 1.6f, 0.0f, 8.0f);
		FMath::Clamp(GetCharacterMovement()->GravityScale += 0.1f, 0.0f, 1.0f);
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Reset"));
		

	}
	
}

void ARealityCharacter::BoolSwitch()
{
	TimerCounter += 1;

	inAir = GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling;
		
	///BunnyHopLogic goes here///
		if (inAir)
		{

			float FAirControl = GetCharacterMovement()->AirControl;
			if (FAirControl < 1)
			{
				FMath::Clamp(GetCharacterMovement()->AirControl += 0.1f, 0.5f, 1.0f);
			}
			
				
			float WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
			if (WalkSpeed < 2000.0f)
			{
				FMath::Clamp(GetCharacterMovement()->MaxWalkSpeed += 50.0f, 600.0f, 2000.0f);
			}
			

			isOnFloor = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("InAir"));
		}

		if (inAir == false)
		{
			isOnFloor = true;
			
			TimerReset();
			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("OnFloor"));

			if (TimerCounter >= 5)
			{
				GetWorldTimerManager().ClearTimer(Handler);
				GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
				TimerCounter = 0;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TimerCleared"));
			}
		}
		
	
}

void ARealityCharacter::OnOverlapEnd(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	
}



//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ARealityCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ARealityCharacter::MoveForward(float Value)
{
	

	ForwardValue = Value;
	
	
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
	if (Value == 1.0f)
	{
		Forward = true;
		Right = false;
		Left = false;
		Back = false;
	}

	if (Value == -1.0f)
	{
		Forward = false;
		Right = false;
		Left = false;
		Back = true;
	}

	if (Value == 0.0f)
	{
		Forward = false;
		Right = false;
		Left = false;
		Back = false;

	}
}

void ARealityCharacter::MoveRight(float Value)
{
	
	
	RightValue = Value;
	

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
	if (Value == 1.0f)
	{
		Right = true;
		Forward = false;
		Left = false;
		Back = false;
	}

	if (Value == -1.0f)
	{
		Forward = false;
		Right = false;
		Left = true;
		Back = false;
	}
}

void ARealityCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARealityCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ARealityCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARealityCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ARealityCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ARealityCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

