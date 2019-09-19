// Fill out your copyright notice in the Description page of Project Settings.

#include "RealityKey.h"


// Sets default values
ARealityKey::ARealityKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Mesh;
	Box->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ARealityKey::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARealityKey::OnOverlapBegin);
}

// Called every frame
void ARealityKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARealityKey::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);
		if (Character)
		{
			bKey = true;
			Destroy();

		}
	
}

