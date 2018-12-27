// Fill out your copyright notice in the Description page of Project Settings.

#include "BoundaryFence.h"
#include "Stokedew_Valley2Character.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"

// Sets default values
ABoundaryFence::ABoundaryFence()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoundaryFence::BeginPlay()
{
	Super::BeginPlay();
	character = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ABoundaryFence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoundaryFence::PurchaseLand()
{
	if (character->GetGold() >= 500)
	{
		character->ChangeGold(-500);
		Destroy();
	}
}