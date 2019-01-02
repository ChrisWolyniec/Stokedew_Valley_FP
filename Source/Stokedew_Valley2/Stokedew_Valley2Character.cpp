// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Stokedew_Valley2Character.h"
#include "Stokedew_Valley2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
#include "Engine.h"
#include "IInteractable.h"
#include "Classes/Landscape.h"
#include "DirtPlot.h"

//Purely for debug
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
// End debug

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AStokedew_Valley2Character

AStokedew_Valley2Character::AStokedew_Valley2Character()
{
	//Player stats
	playerStamina = 100.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AStokedew_Valley2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStokedew_Valley2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	deltaTime = DeltaTime;

	FString seedCountOutput = FString::FromInt(seeds);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, TEXT("Seeds: ") + seedCountOutput);

	FString cropCountOutput = FString::FromInt(crops);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, TEXT("Crops: ") + cropCountOutput);

	FString goldOutput = FString::FromInt(gold);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, TEXT("Gold: ") + goldOutput);


	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));


	FString wheatOutput = FString::FromInt(wheatCount);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("Wheat: ") + wheatOutput);

	FString cornOutput = FString::FromInt(cornCount);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("Corn: ") + cornOutput);

	FString strawberryOutput = FString::FromInt(strawberryCount);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("Strawberries: ") + strawberryOutput);

	FString sunflowerOutput = FString::FromInt(sunflowerCount);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("Sunflowers: ") + sunflowerOutput);



	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));



	FString heldProduceName;

	switch (heldProduceValue)
	{
	case 0: heldProduceName = "Wheat";
		break;
	case 1: heldProduceName = "Corn";
		break;
	case 2: heldProduceName = "Strawberries";
		break;
	case 3: heldProduceName = "Sunflowers";
		break;
	default:
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT("Held Produce: ") + heldProduceName);



	FString equipedToolName;

	switch (equipedTool)
	{
	case 0: equipedToolName = "Hoe";
		break;
	case 1: equipedToolName = "Trowel";
		break;
	case 2: equipedToolName = "Watering Can";
		break;
	case 3: equipedToolName = "Sickle";
		break;
	default:
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT("Held Tool: ") + equipedToolName);



	FString equipedSeedName;

	switch (heldSeed)
	{
	case 0: equipedSeedName = "Wheat";
		break;
	case 1: equipedSeedName = "Corn";
		break;
	case 2: equipedSeedName = "Strawberry";
		break;
	case 3: equipedSeedName = "Sunflower";
		break;
	default:
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT("Held Seed: ") + equipedSeedName);

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, TEXT(" "));
}


void AStokedew_Valley2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AStokedew_Valley2Character::Raycast);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AStokedew_Valley2Character::Raycast);
	PlayerInputComponent->BindAction("ChangeHeldProduce", IE_Pressed, this, &AStokedew_Valley2Character::ChangeHeldProduce);
	PlayerInputComponent->BindAction("ChangeEquipedTool", IE_Pressed, this, &AStokedew_Valley2Character::ChangeEquipedTool);
	PlayerInputComponent->BindAction("ChangeHeldSeed", IE_Pressed, this, &AStokedew_Valley2Character::ChangeHeldSeed);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AStokedew_Valley2Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AStokedew_Valley2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStokedew_Valley2Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStokedew_Valley2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStokedew_Valley2Character::LookUpAtRate);
}

void AStokedew_Valley2Character::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AStokedew_Valley2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AStokedew_Valley2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AStokedew_Valley2Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AStokedew_Valley2Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AStokedew_Valley2Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AStokedew_Valley2Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AStokedew_Valley2Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AStokedew_Valley2Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AStokedew_Valley2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStokedew_Valley2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AStokedew_Valley2Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AStokedew_Valley2Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AStokedew_Valley2Character::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AStokedew_Valley2Character::TouchUpdate);
		return true;
	}
	
	return false;
}

void AStokedew_Valley2Character::ChangePlayerStamina(int amount)
{
	// if action occured then decrease stamina by soso amount
	playerStamina += amount;
}


int AStokedew_Valley2Character::GetSeedCount(int seedType)
{
	if (seedType == 0)
	{
		return wheatSeedCount;
	}
	else if (seedType == 1)
	{
		return cornSeedCount;
	}
	else if (seedType == 2)
	{
		return strawberrySeedCount;
	}
	else if (seedType == 3)
	{
		return sunflowerSeedCount;
	}
	return 17;
}

void AStokedew_Valley2Character::ChangeSeedCount(int value, int cropType)
{
	if (cropType == 0)
	{
		wheatSeedCount += value;
	}
	else if (cropType == 1)
	{
		cornSeedCount += value;
	}
	else if (cropType == 2)
	{
		strawberrySeedCount += value;
	}
	else if (cropType == 3)
	{
		sunflowerSeedCount += value;
	}


}

void AStokedew_Valley2Character::ChangeCropCount(int value, int cropType)
{
	if (cropType == 0)
	{
		wheatCount += value;
	}
	else if (cropType == 1)
	{
		cornCount += value;
	}
	else if (cropType == 2)
	{
		strawberryCount += value;
	}
	else if (cropType == 3)
	{
		sunflowerCount += value;
	}
}

void AStokedew_Valley2Character::SetPlayerLocation(float x, float y, float z)
{
	FVector location = { x, y, z };
	TeleportTo(location, GetActorRotation());
}

void AStokedew_Valley2Character::Sleep(bool sleep)
{
	sleeping = sleep;
}

bool AStokedew_Valley2Character::GetSleep()
{
	return sleeping;
}

int AStokedew_Valley2Character::GetGold()
{
	return gold;
}

void AStokedew_Valley2Character::ChangeGold(int goldChange)
{
	gold += goldChange;
}

void AStokedew_Valley2Character::Raycast()
{
	FHitResult* hitResult = new FHitResult();
	FVector startTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector endTrace = (forwardVector * 500.0f) + startTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();


	if (GetWorld()->LineTraceSingleByChannel(*hitResult, startTrace, endTrace, ECC_Visibility, *CQP))
	{
		DrawDebugLine(GetWorld(), startTrace, endTrace, FColor(255, 0, 0), true);

		if (hitResult->GetActor() != NULL)
		{
			if (Cast<IIInteractable> (hitResult->GetActor()) != nullptr)
			{
				IIInteractable* interactable = Cast<IIInteractable>(hitResult->GetActor());
				interactable->Interact();
			}
			else if (Cast<ALandscape>(hitResult->GetActor()) != nullptr && equipedTool == 0)
			{
				ALandscape* landscape = Cast<ALandscape>(hitResult->GetActor());
				hitResult->ImpactPoint;

				UWorld* const World = GetWorld();

				FVector editedImpactPoint = hitResult->ImpactPoint;

				editedImpactPoint.X = editedImpactPoint.X - 55;
				editedImpactPoint.Y = editedImpactPoint.Y - 55;

				editedImpactPoint.X = editedImpactPoint.X - ((int)editedImpactPoint.X % 110);
				editedImpactPoint.Y = editedImpactPoint.Y - ((int)editedImpactPoint.Y % 110);
				editedImpactPoint.Z = 170.0f;

				if (editedImpactPoint.X > 0)
				{
					editedImpactPoint.X += 110.0f;
				}
				if (editedImpactPoint.Y > 0)
				{
					editedImpactPoint.Y += 110.0f;
				}

				const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
				const FVector SpawnLocation = editedImpactPoint;
				ADirtPlot* newPlot = World->SpawnActor<ADirtPlot>(PlotClass, SpawnLocation, SpawnRotation);
			}

		}
	}

	delete hitResult;
	delete CQP;
}




void AStokedew_Valley2Character::SetNight(bool nightPassed)
{
	night = nightPassed;
}

int AStokedew_Valley2Character::GetEquipedTool()
{
	return equipedTool;
}

void AStokedew_Valley2Character::ChangeEquipedTool()
{
	equipedTool++;

	if (equipedTool == 4)
	{
		equipedTool = 0;
	}
}


int AStokedew_Valley2Character::GetHeldSeed()
{
	return heldSeed;
}

void AStokedew_Valley2Character::ChangeHeldSeed()
{
	heldSeed++;

	if (heldSeed == 4)
	{
		heldSeed = 0;
	}
}


void AStokedew_Valley2Character::ChangeHeldProduce()
{
	heldProduceValue++;
	if (heldProduceValue == 4)
	{
		heldProduceValue = 0;
	}
}

int AStokedew_Valley2Character::GetHeldProduce()
{
	return heldProduceValue;
}

int AStokedew_Valley2Character::GetWheatCount()
{
	return wheatCount;
}
void AStokedew_Valley2Character::ChangeWheatCount(int change)
{
	wheatCount += change;
}

void AStokedew_Valley2Character::ChangeCornCount(int change)
{
	cornCount += change;
}
int AStokedew_Valley2Character::GetCornCount()
{
	return cornCount;
}

int AStokedew_Valley2Character::GetStrawberryCount()
{
	return strawberryCount;
}
void AStokedew_Valley2Character::ChangeStrawberryCount(int change)
{
	strawberryCount += change;
}

int AStokedew_Valley2Character::GetSunflowerCount()
{
	return sunflowerCount;
}
void AStokedew_Valley2Character::ChangeSunflowerCount(int change)
{
	sunflowerCount += change;
}