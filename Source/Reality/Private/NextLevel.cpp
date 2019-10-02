// Fill out your copyright notice in the Description page of Project Settings.

#include "NextLevel.h"
#include "RealityGameMode.h"


// Sets default values
ANextLevel::ANextLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
}

// Called when the game starts or when spawned
void ANextLevel::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::OnOverlapBegin);

}

// Called every frame
void ANextLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANextLevel::OnOverlapBegin(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Character = Cast<ARealityCharacter>(OtherActor);
	ARealityGameMode* GameMode = Cast<ARealityGameMode>(GetWorld()->GetAuthGameMode());
	if (Character != nullptr)
	{
		Character->ShowScore();
		if (GameMode != nullptr)
		{
			
			GetWorldTimerManager().ClearTimer(GameMode->PlayerTime);
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &ANextLevel::OpenLevel, 2.0f, false);
				
				
		}
	}
	
}

void ANextLevel::OpenLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), Level);
}

