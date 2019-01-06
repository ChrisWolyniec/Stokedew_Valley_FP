// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"
#include "Stokedew_Valley2Character.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	character = Cast<AStokedew_Valley2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::PurchaseFromPlayer()
{
	int heldProduceValue = character->GetHeldProduce();

	switch (heldProduceValue)
	{
	case 0: 
		wheatCount = character->GetWheatCount();
		if (wheatCount >= 5)
		{
			character->ChangeGold(10);
			character->ChangeWheatCount(-5);
		}
		else if(wheatCount > 0)
		{
			character->ChangeGold(2);
			character->ChangeWheatCount(-1);
		}
		break;
	case 1:
		cornCount = character->GetCornCount();
		if (cornCount >= 5)
		{
			character->ChangeGold(15);
			character->ChangeCornCount(-5);
		}
		else if (cornCount > 0)
		{
			character->ChangeGold(3);
			character->ChangeCornCount(-1);
		}
		break;
	case 2:
		strawberryCount = character->GetStrawberryCount();
		if (strawberryCount >= 15)
		{
			character->ChangeGold(10);
			character->ChangeStrawberryCount(-15);
		}
		else if (strawberryCount >= 3)
		{
			character->ChangeGold(2);
			character->ChangeStrawberryCount(-3);
		}
		break;
	case 3:
		sunflowerCount = character->GetSunflowerCount();
		if (sunflowerCount >= 5)
		{
			character->ChangeGold(10);
			character->ChangeSunflowerCount(-5);
		}
		else if (sunflowerCount > 0)
		{
			character->ChangeGold(2);
			character->ChangeSunflowerCount(-1);
		}
		break;
	default:
		break;
	}
}

void AShop::SellToPlayer()
{
	int heldProduceValue = character->GetHeldProduce();
	int playersGold = character->GetGold();

	switch (heldProduceValue)
	{
	case 0:
		if (playersGold >= 5)
		{
			character->ChangeGold(-10);
			character->ChangeWheatCount(5);
		}
		else if (playersGold > 0)
		{
			character->ChangeGold(-2);
			character->ChangeWheatCount(1);
		}
		break;
	case 1:
		if (playersGold >= 5)
		{
			character->ChangeGold(-15);
			character->ChangeCornCount(5);
		}
		else if (playersGold > 0)
		{
			character->ChangeGold(-3);
			character->ChangeCornCount(1);
		}
		break;
	case 2:
		if (playersGold >= 15)
		{
			character->ChangeGold(-10);
			character->ChangeStrawberryCount(15);
		}
		else if (playersGold >= 3)
		{
			character->ChangeGold(-2);
			character->ChangeStrawberryCount(3);
		}
		break;
	case 3:
		if (playersGold >= 5)
		{
			character->ChangeGold(-10);
			character->ChangeSunflowerCount(5);
		}
		else if (playersGold > 0)
		{
			character->ChangeGold(-2);
			character->ChangeSunflowerCount(1);
		}
		break;
	default:
		break;
	}
}

void AShop::Interact()
{
	if (seller)
	{
		SellToPlayer();
	}
	else
	{
		PurchaseFromPlayer();
	}
}