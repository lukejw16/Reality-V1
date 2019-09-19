// Fill out your copyright notice in the Description page of Project Settings.

#include "VelocityChange.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AVelocityChange::AVelocityChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(SphereMesh);
	RootComponent = SphereMesh;
	bIsForward = true;
	bIsPlus = false;
}

// Called when the game starts or when spawned
void AVelocityChange::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AVelocityChange::OnOverlapBegin);

}

// Called every frame
void AVelocityChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVelocityChange::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character)
	{
		const FVector ForwardDir = Character->GetRootComponent()->GetForwardVector();
		const FVector ForwardWithUp = (Character->GetRootComponent()->GetUpVector());
		const FVector ForwardWithZ = (ForwardDir + FVector(0, 0, 1));
		if (bIsForward == true)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity = ForwardDir * SpeedChange;
		}

		if (bIsPlus == true && bIsForward == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity += ForwardDir * SpeedChange;
		}

		if (bIsForward == false && bIsPlus == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity += ForwardWithUp * SpeedChange;
		}

	}
	
	
}