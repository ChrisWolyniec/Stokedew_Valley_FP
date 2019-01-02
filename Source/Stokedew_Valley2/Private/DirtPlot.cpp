// Fill out your copyright notice in the Description page of Project Settings.

#include "DirtPlot.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"
#include "Crop.h"
#include "Stokedew_Valley2Character.h"
#include "ctime"

// Sets default values
ADirtPlot::ADirtPlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADirtPlot::BeginPlay()
{
	Super::BeginPlay();
	character = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ADirtPlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirtPlot::SpawnCrop()
{
	//const FRotator SpawnRotation = GetActorRotation();
	if (!planted && character->GetEquipedTool() == 1)
	{
		int seedType = character->GetHeldSeed();
		if (character->GetSeedCount(seedType) > 0)
		{
			int stamLoss = -5;
			planted = true;
			character->ChangeSeedCount(-1, seedType);
			UWorld* const World = GetWorld();

			//srand(time(NULL)); TODO srand
			const FRotator SpawnRotation = FRotator(0.0f, (rand() % 360), 0.0f);
			const FVector SpawnLocation = GetActorLocation();
			crop = World->SpawnActor<ACrop>(CropClass, SpawnLocation, SpawnRotation);
			crop->myPlot = this;
			crop->SetCropType(seedType);
		}
	}
	else
	{
		if (crop != NULL)
		{
			crop->Interact();
		}
	}
}

void ADirtPlot::Interact()
{
	SpawnCrop();
}

