// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crop.generated.h"

class ADirtPlot;

UCLASS()
class STOKEDEW_VALLEY2_API ACrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UStaticMeshComponent * MeshComponent;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateGrowth(float DeltaTime);

	void Harvest();

	float deltaTime = 0.0f;
	float timeSincePlanted = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Stages")
	UStaticMesh* StageOne;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Stages")
	UStaticMesh* StageTwo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Stages")
	UStaticMesh* StageThree;

	ADirtPlot* myPlot;
	
private:
	int stage = 1;
	
};
