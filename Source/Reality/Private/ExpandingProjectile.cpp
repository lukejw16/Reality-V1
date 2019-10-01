// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpandingProjectile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
AExpandingProjectile::AExpandingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetupAttachment(Mesh);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AExpandingProjectile::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AExpandingProjectile::OnOverlapEnd);
}
	


void AExpandingProjectile::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	if (Character != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AExpandingProjectile::DepleteEnergy, 0.1f, true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlap"));
	}

}

void AExpandingProjectile::OnOverlapEnd(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

// Called when the game starts or when spawned
void AExpandingProjectile::BeginPlay()
{
	Super::BeginPlay();
	Scale = Mesh->GetComponentScale();
	OriginalScale = Mesh->GetComponentScale();
	
	CollisionComp->SetSphereRadius(radius);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AExpandingProjectile::ScaleIncrease, 0.01f, true);
}

// Called every frame
void AExpandingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AExpandingProjectile::ScaleIncrease()
{
	if (timecounter++, timecounter < 200)
	{
		Mesh->SetWorldScale3D(Scale + FVector(0.01f, 0.01f, 0.01f));
		Mesh->SetRelativeScale3D(Scale + FVector(0.01f, 0.01f, 0.01f));
		Scale = Scale + FVector(0.01f, 0.01f, 0.01f);
		CollisionComp->SetSphereRadius(radius + 160);
	}
	else 
	{
		bSrink = true;
		
	}

	if (bSrink == true) 
	{
		
		Mesh->SetWorldScale3D(Scale + FVector(0.01f, 0.01f, 0.01f));
		Mesh->SetRelativeScale3D(Scale - FVector(0.01f, 0.01f, 0.01f));
		Scale = Scale - FVector(0.01f, 0.01f, 0.01f);
		
		if (Scale.Size() < OriginalScale.Size())
		{
			if (Environmental == true)
			{
				bSrink = false;
				timecounter = 0;
			}
			else {
				GetWorldTimerManager().ClearTimer(Timer);
				Destroy();
			}
			
		}
	}
	

}

void AExpandingProjectile::DepleteEnergy()
{
	Character = Cast<ARealityCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Character != nullptr)
	{
		Character->EnergyMeter -= 0.05f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Overlap"));

	}
		
	
}

