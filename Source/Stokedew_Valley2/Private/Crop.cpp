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

	wheatMat = CreateDefaultSubobject<UMaterial>(TEXT("Wheat Material"));
	cornMat = CreateDefaultSubobject<UMaterial>(TEXT("Corn Material"));
	strawberryMat = CreateDefaultSubobject<UMaterial>(TEXT("Strawberry Material"));
	sunflowerMat = CreateDefaultSubobject<UMaterial>(TEXT("Sunflower Material"));
}

// Called when the game starts or when spawned
void ACrop::BeginPlay()
{
	Super::BeginPlay();
	character = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	MeshComponent->SetStaticMesh(StageOne);
	MeshComponent->SetMaterial(0, wheatMat);
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
	if (!night && watered)
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
	if (stage == 3 && !harvested)
	{
		myPlot->planted = false;
		int seedsGained = (rand() % 3);
		int cropsGained = (rand() % 3) + 2;
		character->ChangeSeedCount(seedsGained, cropType);
		character->ChangeCropCount(cropsGained, cropType);
		harvested = true;
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

void ACrop::Interact()
{
	if (character->GetEquipedTool() == 2 && !watered)
	{
		watered = true;
	}
	else if (character->GetEquipedTool() == 3)
	{
		Harvest();
	}
}

void ACrop::SetCropType(int cropTypePassed)
{
	cropType = cropTypePassed;
	if (cropType == 0)
	{
		MeshComponent->SetMaterial(0, wheatMat);
	}
	else if (cropType == 1)
	{
		MeshComponent->SetMaterial(0, cornMat);
	}
	else if (cropType == 2)
	{
		MeshComponent->SetMaterial(0, strawberryMat);
	}
	else if (cropType == 3)
	{
		MeshComponent->SetMaterial(0, sunflowerMat);
	}
}