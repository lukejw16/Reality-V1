// Fill out your copyright notice in the Description page of Project Settings.

#include "RealitySurf.h"
#include "RealityCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

class ARealityCharacter;
// Sets default values
ARealitySurf::ARealitySurf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(SphereMesh);
	RootComponent = SphereMesh;
	bIsForward = false;
}

// Called when the game starts or when spawned
void ARealitySurf::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARealitySurf::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARealitySurf::OnOverlapEnd);
}

// Called every frame
void ARealitySurf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARealitySurf::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character)
	{
		Character->JumpCurrentCount = 0;
		Character->GetCharacterMovement()->AirControl = 1.0f;
		FMath::Clamp(Character->GetCharacterMovement()->MaxWalkSpeed += 20.0f, 600.0f, 2000.0f);
		Character->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		Character->GetCharacterMovement()->GroundFriction = 0.0f;
		Character->GetCharacterMovement()->GravityScale = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));

		GetWorld()->GetTimerManager().SetTimer(Character->Handler, this, &ARealitySurf::StartGame, 0.5f, true);
		Character->OverlappedWithRamp = true;
	}
	
	
}

void ARealitySurf::OnOverlapEnd(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character)
	{
		Character->OverlappedWithRamp = false;
		StartGame();

	}
	
	
}

void ARealitySurf::ResetHandle()
{
	if (bIsForward == true)
	{
		Character->GetCharacterMovement()->AirControl = 0.5f;
		Character->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		Character->GetCharacterMovement()->GroundFriction = 8.0f;
		Character->GetCharacterMovement()->GravityScale = 1.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("EndOverlap"));
	}
}

void ARealitySurf::StartGame()
{
	if (Character)
	{
		Character->GetCharacterMovement()->GravityScale = 1.0f;

	}
	
	
}
