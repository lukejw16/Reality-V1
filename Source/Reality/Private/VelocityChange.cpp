// Fill out your copyright notice in the Description page of Project Settings.

#include "VelocityChange.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
	if (Character != nullptr)
	{
		const FVector ForwardDir = Character->GetRootComponent()->GetForwardVector();
		const FVector ForwardWithUp = (Character->GetRootComponent()->GetUpVector());
		const FVector ForwardWithZ = (ForwardDir + FVector(0, 0, 1));
		const FVector Velocity = Character->GetCharacterMovement()->Velocity;
		const FVector MinVelocity(300.0f, 300.0f, 300.0f);
		float speed = Velocity.Size();
		FString TheFloatStr = FString::SanitizeFloat(speed);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);

		if (BoostSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, BoostSound, 0.5f, 1.0f, 0.0f, nullptr, nullptr);
		}
		

		if (bIsForward == true )
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity = ForwardDir * SpeedChange;
			GetWorld()->GetTimerManager().SetTimer(Character->FOVTimer, this, &AVelocityChange::ChangeFieldOfView, 0.02f, true);
			
		}

		if (bIsPlus == true && bIsForward == false && speed < 3000)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity += ForwardDir * SpeedChange;
			GetWorld()->GetTimerManager().SetTimer(Character->FOVTimer, this, &AVelocityChange::ChangeFieldOfView, 0.02f, true);
			
		}

		if (bIsPlus == true && bIsForward == false && speed > 5000)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity = ForwardDir * SpeedChange;
			GetWorld()->GetTimerManager().SetTimer(Character->FOVTimer, this, &AVelocityChange::ChangeFieldOfView, 0.02f, true);
			
		}

		if (bIsForward == false && bIsPlus == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity += ForwardWithUp * SpeedChange;
			GetWorld()->GetTimerManager().SetTimer(Character->FOVTimer, this, &AVelocityChange::ChangeFieldOfView, 0.02f, true);;
			
		}

		if (bIsForward == false && bIsPlus == false && speed > 500)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));
			FMath::Clamp(SpeedChange, 600.0f, 2000.0f);
			Character->GetCharacterMovement()->Velocity = ForwardWithUp * SpeedChange;
			GetWorld()->GetTimerManager().SetTimer(Character->FOVTimer, this, &AVelocityChange::ChangeFieldOfView, 0.02f, true);
			
		}
	}
	
	
}

void AVelocityChange::ChangeFieldOfView()
{
	Character->ChangeFieldofView();
}
