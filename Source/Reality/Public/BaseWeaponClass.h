// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RealityCharacter.h"
#include "BaseWeaponClass.generated.h"

UENUM(BlueprintType)
enum WeaponsN 
{
	P     UMETA(DisplayName = "P"),
	SM      UMETA(DisplayName = "SM"),
	S   UMETA(DisplayName = "S"),
};



UCLASS()
class REALITY_API ABaseWeaponClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		ARealityCharacter* Character;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		class UBoxComponent* Box;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TEnumAsByte<WeaponsN> Weapon;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float YawValue;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float RollValue;
};
