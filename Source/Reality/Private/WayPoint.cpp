// Fill out your copyright notice in the Description page of Project Settings.

#include "WayPoint.h"
#include "AICharacter.h"


// Sets default values
AWayPoint::AWayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AWayPoint::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWayPoint::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AAICharacter* Character = nullptr;

	if (OtherActor != nullptr)
	{
		Character = Cast<AAICharacter>(OtherActor);
		if (Character != nullptr)
		{
			Character->NextWaypoint = NextWaypoint;
			//Controller = Cast<AAI_Controller>(Character->GetController());
			//Controller->MoveToActor(NextWaypoint, 5.0f);
		}
	}
}

