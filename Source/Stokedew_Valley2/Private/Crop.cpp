// Fill out your copyright notice in the Description page of Project Settings.

#include "Crop.h"
#include "Stokedew_Valley2Character.h"
#include "Engine.h"
#include "DirtPlot.h"


// Sets default values
ACrop::ACrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CropStage"));
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	name = "crop boi";
	IsStackable = true;
	itemDescription = "this is a crop";
	IsConsumable = false;
	maxStackable = 99;
}

// Called when the game starts or when spawned
void ACrop::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetStaticMesh(StageOne);
	character = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

// Called every frame
void ACrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;

	UpdateGrowth(DeltaTime);

}

void ACrop::UpdateGrowth(float DeltaTime)
{
	if (!night)
	{
		timeSincePlanted += DeltaTime;
		if (timeSincePlanted > 6)
		{
			MeshComponent->SetStaticMesh(StageThree);
			stage = 3;
		}
		else if (timeSincePlanted > 3)
		{
			MeshComponent->SetStaticMesh(StageTwo);
			stage = 2;
		}
	}
}

void ACrop::Harvest()
{
	if (stage == 3)
	{
		myPlot->planted = false;
		int seedsGained = (rand() % 3);
		int cropsGained = (rand() % 3) + 2;
		character->ChangeSeedCount(seedsGained);
		character->ChangeCropCount(cropsGained);
		Destroy();
	}
}

void ACrop::SetNight(bool nightPassed)
{
	night = nightPassed;
}

bool ACrop::GetNight()
{
	return night;
}