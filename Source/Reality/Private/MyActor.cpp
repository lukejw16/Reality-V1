// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"


// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	FireLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FireLocation"));
	RootComponent = Mesh;
	FireLocation->SetupAttachment(Mesh);
	
	Delay = 1.0f;
	
	index = 0;
	
	
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	indexMax = Path->NewLocation.Num();
	indexMax--;
	OrginalLocation = GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AMyActor::SelectMovement, 2.0, true);

	

	
	}

	

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::Spawn()
{

	const FRotator SpawnRotation = FireLocation->GetComponentRotation();
	const FVector SpawnLocation = ((FireLocation != nullptr) ? FireLocation->GetComponentLocation() : GetActorLocation());

	
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			
			world->SpawnActor<AEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Fire"));

			
			
			
			
		}

	
}

void AMyActor::move()
{
	

	FVector CurrentLocation = Path->GetActorTransform().GetLocation();
	FVector PathLocationPoint = CurrentLocation + Path->NewLocation[index];
	

	switch (switchonint)

	{
		case 0:
			
			
			SetActorLocation(CurrentLocation, false, nullptr, ETeleportType::None);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Start"));
			switchonint = 1;
		break;
		case 1:
			
			if (index >= indexMax)
			{
				switchonint = 2;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("index >= indexMax"));
				

			}
			else
			{
				
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Increment"));
				index++;
				SetActorLocation(PathLocationPoint, false, nullptr, ETeleportType::None);
			}
		break;
		case 2:
			
			if (index == 0)
			{
				switchonint = 0;
				
			}
			else
			{
				
				index--;

				SetActorLocation(PathLocationPoint, false, nullptr, ETeleportType::None);
				
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Decrement"));
				
				
				
			}
		break;
	default:
		break;
	}
}

void AMyActor::RandomPathPoint()
{
	int32 min = 0;
	int32 max = Path->NewLocation.Num() - 1;

	int random = FMath::RandRange(min, max);

	FVector CurrentLocation = Path->GetActorTransform().GetLocation();
	FVector PathLocationPoint = CurrentLocation + Path->NewLocation[random];

	SetActorLocation(PathLocationPoint, false, nullptr, ETeleportType::None);
}

void AMyActor::SelectMovement()
{
	if (bRandom == true)
	{
		RandomPathPoint();
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMyActor::Spawn, Delay, false);
	}
	else
	{
		move();
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMyActor::Spawn, Delay, false);
	}


}

