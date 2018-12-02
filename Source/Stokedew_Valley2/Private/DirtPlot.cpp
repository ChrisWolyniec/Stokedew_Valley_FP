// Fill out your copyright notice in the Description page of Project Settings.

#include "DirtPlot.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"
#include "Crop.h"
#include "ctime"
#include "Stokedew_Valley2Character.h"

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
	if (Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) != nullptr)
	{
		player = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}

// Called every frame
void ADirtPlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirtPlot::SpawnCrop()
{
	UWorld* const World = GetWorld();
	int stamLoss = -5;
	player->ChangePlayerStamina(stamLoss);
	//const FRotator SpawnRotation = GetActorRotation();
	//srand(time(NULL)); TODO srand
	const FRotator SpawnRotation = FRotator(0.0f, (rand() % 360), 0.0f);
	const FVector SpawnLocation = GetActorLocation();
	World->SpawnActor<ACrop>(CropClass, SpawnLocation, SpawnRotation);

}

