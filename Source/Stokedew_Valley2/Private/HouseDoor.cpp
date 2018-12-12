// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseDoor.h"
#include "Stokedew_Valley2Character.h"
#include "Engine.h"

// Sets default values
AHouseDoor::AHouseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHouseDoor::BeginPlay()
{
	Super::BeginPlay();
	character = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

// Called every frame
void AHouseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHouseDoor::Teleport()
{
	character->SetPlayerLocation(xCoord, yCoord, zCoord);
}