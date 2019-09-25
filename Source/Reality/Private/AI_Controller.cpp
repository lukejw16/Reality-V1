// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Controller.h"
#include "Perception/AIPerceptionComponent.h"
#include "AICharacter.h"
#include "WayPoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "RealityCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_Controller::AAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem"));
	}

	
	

}

void AAI_Controller::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);
}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AAICharacter* Character = Cast<AAICharacter>(GetPawn());

	if (DistanceToPlayer > AISightRadius)
	{
		
		bPlayerIsDetected = false;
		Character->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}

	if (Character->NextWaypoint != nullptr && bPlayerIsDetected == false)
	{
		MoveToActor(Character->NextWaypoint, 5.0f);
		
	}
	else if (bPlayerIsDetected == true)
	{
		ARealityCharacter* Player = Cast<ARealityCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Character->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		
		
		MoveToActor(Player, 600.0f);

		

	}	
	
}

FRotator AAI_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr )
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAI_Controller::OnPawnDetected(TArray<AActor*> DetectedPawn)
{
	for (size_t i = 0; i < DetectedPawn.Num(); i++)
	{
		
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawn[i]);
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AAI_Controller::Shoot, WeaponDelay, true);
	}
	
	bPlayerIsDetected = true;
}

void AAI_Controller::Shoot()
{
	ARealityCharacter* Player = Cast<ARealityCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (Player != nullptr)
	{
		AAICharacter* Character = Cast<AAICharacter>(GetPawn());
		SetFocus(Player);
		Character->FireWeapon();

		
	}
}


