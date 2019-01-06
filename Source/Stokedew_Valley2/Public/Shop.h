// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "Shop.generated.h"

class AStokedew_Valley2Character;

UCLASS()
class STOKEDEW_VALLEY2_API AShop : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();
	AStokedew_Valley2Character* character;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PurchaseFromPlayer();
	void SellToPlayer();

	virtual void Interact();

private:
	int wheatCount;
	int cornCount;
	int strawberryCount;
	int sunflowerCount;

	UPROPERTY(EditAnywhere, Category = "Shop Type")
	bool seller;
};
