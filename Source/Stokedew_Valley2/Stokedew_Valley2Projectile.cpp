// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Stokedew_Valley2Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DirtPlot.h"
#include "Crop.h"
#include "HouseDoor.h"
#include "Bed.h"
#include "BoundaryFence.h"

//Purely for debug
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
// End debug

AStokedew_Valley2Projectile::AStokedew_Valley2Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AStokedew_Valley2Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AStokedew_Valley2Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics

	if (Cast<ADirtPlot>(OtherActor) != nullptr)
	{
		ADirtPlot* plot = Cast<ADirtPlot>(OtherActor);
		plot->SpawnCrop();
		Destroy();
	}
	else if (Cast<ACrop>(OtherActor) != nullptr)
	{
		ACrop* crop = Cast<ACrop>(OtherActor);
		crop->Harvest();
		Destroy();
	}
	else if (Cast<AHouseDoor>(OtherActor) != nullptr)
	{
		AHouseDoor* door = Cast<AHouseDoor>(OtherActor);
		door->Teleport();
		Destroy();
	}
	else if (Cast<ABed>(OtherActor) != nullptr)
	{
		if (night == true)
		{
			ABed* bed = Cast<ABed>(OtherActor);
			bed->Sleep(true);
			Destroy();
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Sleeping"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Not Night"));
		}
	}
	else if (Cast<ABoundaryFence>(OtherActor) != nullptr)
	{
		ABoundaryFence* fence = Cast<ABoundaryFence>(OtherActor);
		fence->PurchaseLand();
		Destroy();
	}


	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void AStokedew_Valley2Projectile::SetNight(bool nightPassed)
{
	night = nightPassed;
}
