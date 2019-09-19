// Fill out your copyright notice in the Description page of Project Settings.

#include "RealityDoor.h"
#include "RealityProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"


// Sets default values
ARealityDoor::ARealityDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Mesh;
	BoxCollision->SetupAttachment(Mesh);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	PSC = PS.Object;
}

// Called when the game starts or when spawned
void ARealityDoor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARealityDoor::OnOverlapBegin);
	
}


// Called every frame
void ARealityDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARealityDoor::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Projectile = Cast<ARealityProjectile>(OtherActor);
	Character = Cast<ARealityCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (Projectile && bKeyNeeded == false)
	{
		ChangeEnergyMeter();
		SpawnEmitter();
		Projectile->Destroy();
		Destroy();
		
	}

	if (Projectile && bKeyNeeded)
	{
	bGotKey = Key->bKey;
		if (bGotKey)
		{
			ChangeEnergyMeter();
			SpawnEmitter();
			Projectile->Destroy();
			Destroy();
		}
		

	}

}

void ARealityDoor::SpawnEmitter()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSC, GetActorLocation());
}

void ARealityDoor::ChangeEnergyMeter()
{
	
	Character->EnergyMeter += 0.2f;
}



