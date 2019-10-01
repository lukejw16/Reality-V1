// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASplineManager::ASplineManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	YourSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	BoxCollision->SetupAttachment(YourSplineComponent);
	SphereMesh->SetupAttachment(BoxCollision);
	RootComponent = YourSplineComponent;

}

// Called when the game starts or when spawned
void ASplineManager::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASplineManager::OnOverlapBegin);
	
}

// Called every frame
void ASplineManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineManager::AddToSpline()
{
	if (Multiple < 1)
	{
		Multiple += 0.0005;

		SplineLegnth = YourSplineComponent->GetSplineLength();
		Distance = SplineLegnth * Multiple;
		Points = YourSplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SplineStart"));
		Character->SetActorLocation(Points, true, 0, ETeleportType::None);
		Character->GetCharacterMovement()->GravityScale = 0.5f;
	}

	if (Multiple >= 1)
	{
		Character->GetCharacterMovement()->GravityScale = 1.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SpineEnd"));
		GetWorldTimerManager().ClearTimer(Handler);
		Multiple = 0;
	}
	
}

void ASplineManager::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(Handler, this, &ASplineManager::AddToSpline, 0.001f, true);
	}
	
	
	
}

