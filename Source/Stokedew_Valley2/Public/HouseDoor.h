// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseDoor.generated.h"

class AStokedew_Valley2Character;


UCLASS()
class STOKEDEW_VALLEY2_API AHouseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHouseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Teleport();
	AStokedew_Valley2Character* character;

	UPROPERTY(EditAnywhere, Category = "Teleport")
		float xCoord;

	UPROPERTY(EditAnywhere, Category = "Teleport")
		float yCoord;

	UPROPERTY(EditAnywhere, Category = "Teleport")
		float zCoord;
	
	
};
