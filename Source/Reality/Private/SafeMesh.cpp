// Fill out your copyright notice in the Description page of Project Settings.

#include "SafeMesh.h"


// Sets default values
ASafeMesh::ASafeMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(SphereMesh);
	RootComponent = SphereMesh;
}

// Called when the game starts or when spawned
void ASafeMesh::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASafeMesh::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASafeMesh::OnOverlapEnd);
}

// Called every frame
void ASafeMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASafeMesh::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character) {
		Character->bSafeSpot = true;
	}
	
}

void ASafeMesh::OnOverlapEnd(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character)
	{
		Character->bSafeSpot = false;
	}
	
}

