// Fill out your copyright notice in the Description page of Project Settings.

#include "RealityCore.h"


// Sets default values
ARealityCore::ARealityCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Mesh;
	Box->SetupAttachment(Mesh);
	Health = 100.0f;
}

// Called when the game starts or when spawned
void ARealityCore::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARealityCore::OnOverlapBegin);
}

// Called every frame
void ARealityCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARealityCore::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Projectile = Cast<ARealityProjectile>(OtherActor);
	if (Projectile)
	{
		Character = Cast<ARealityCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Character)
		{
			damageTaken = Character->Damage;
			Health -= damageTaken;
			if (Health <= 0)
			{
				Projectile->Destroy();
				Destroy();
			}
			if (Health > 0)
			{
				Projectile->Destroy();
			}
		}
	}
}

