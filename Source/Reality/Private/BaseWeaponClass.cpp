// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeaponClass.h"


// Sets default values
ABaseWeaponClass::ABaseWeaponClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Mesh;
	Box->SetupAttachment(Mesh);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PitchValue = 0.0f;
	YawValue = 0.0f;
	RollValue = 0.0f;
}

// Called when the game starts or when spawned
void ABaseWeaponClass::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeaponClass::OnOverlapBegin);
}

// Called every frame
void ABaseWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void ABaseWeaponClass::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);

	if (Character)
	{
		switch (Weapon)
		{
		case P:
			Character->WeaponType = Pistol;
			Character->WeaponSelection();
			Destroy();
			break;
		case SM:
			Character->WeaponType = SMG;
			Character->WeaponSelection();
			Character->WeaponDelay = 0.3f;
			Destroy();
			break;
		case S:
			Character->WeaponType = Shotgun;
			Character->WeaponSelection();
			Destroy();
			break;
		default:
			break;
		}
	}
	
	
}

