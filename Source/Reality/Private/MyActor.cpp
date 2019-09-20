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
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMyActor::Spawn, Delay, true);
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

