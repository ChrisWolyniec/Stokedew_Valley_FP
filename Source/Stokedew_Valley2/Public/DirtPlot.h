// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DirtPlot.generated.h"

class ACrop;

UCLASS()
class STOKEDEW_VALLEY2_API ADirtPlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADirtPlot();
	bool planted = false;
	ACrop* crop;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnCrop();

	UPROPERTY(EditDefaultsOnly, Category = Crop)
	TSubclassOf<class ACrop> CropClass;
	
	
};
