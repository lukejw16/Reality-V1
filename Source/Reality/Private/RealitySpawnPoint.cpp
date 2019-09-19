// Fill out your copyright notice in the Description page of Project Settings.

#include "RealitySpawnPoint.h"


// Sets default values
ARealitySpawnPoint::ARealitySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
}

// Called when the game starts or when spawned
void ARealitySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARealitySpawnPoint::OnOverlapBegin);
}

// Called every frame
void ARealitySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARealitySpawnPoint::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character)
	{
		UpdateLocation();
	}
}

void ARealitySpawnPoint::UpdateLocation()
{
	LocationArray.Add(GetActorLocation());
	Character->SpawnLocation = GetActorLocation();
}

