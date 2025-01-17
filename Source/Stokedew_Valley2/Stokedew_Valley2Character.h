// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Stokedew_Valley2Character.generated.h"

class UInputComponent;
class ADirtPlot;

UCLASS(config=Game)
class AStokedew_Valley2Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AStokedew_Valley2Character();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AStokedew_Valley2Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "DirtPlot")
	TSubclassOf<class ADirtPlot> PlotClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	float deltaTime = 0;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }










private:
	UPROPERTY(EditAnywhere, Category = "Seeds and Crops")
		int seeds;
	UPROPERTY(EditAnywhere, Category = "Seeds and Crops")
		int crops;

	void Tick(float DeltaTime);

	void Raycast();

public:

	enum EquipedTool
	{
		Hoe = 0,
		WateringCan,
		Throwel,
		Sickle
	};




	void ChangeSeedCount(int value, int cropType);
	int GetSeedCount(int seedType);

	void ChangeCropCount(int value, int cropType);
	void SetPlayerLocation(float x, float y, float z);


	UFUNCTION(BlueprintCallable, Category = "Sleep")
		void Sleep(bool sleep);

	UFUNCTION(BlueprintCallable, Category = "Sleep")
		bool GetSleep();

	UPROPERTY(EditAnywhere, Category = "Sleep")
	bool sleeping = false;



	UFUNCTION(BlueprintCallable, Category = "Money")
	int GetGold();

	UFUNCTION(BlueprintCallable, Category = "Money")
	void ChangeGold(int goldChange);

	UPROPERTY(EditAnywhere, Category = "Money")
	int gold;





	UPROPERTY(EditAnywhere, Category = "Time")
	bool night = false;

	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetNight(bool nightPassed);




	UFUNCTION(BlueprintCallable, Category = "Equiped")
	int GetEquipedTool();
	void ChangeEquipedTool();

	UFUNCTION(BlueprintCallable, Category = "Equiped")
	int GetHeldProduce();
	void ChangeHeldProduce();

	UFUNCTION(BlueprintCallable, Category = "Equiped")
	int GetHeldSeed();
	void ChangeHeldSeed();





	int GetWheatCount();
	void ChangeWheatCount(int change);

	int GetCornCount();
	void ChangeCornCount(int change);

	int GetStrawberryCount();
	void ChangeStrawberryCount(int change);

	int GetSunflowerCount();
	void ChangeSunflowerCount(int change);



	UPROPERTY(EditAnywhere, Category = "Seeds")
	int wheatSeedCount;

	UPROPERTY(EditAnywhere, Category = "Seeds")
	int cornSeedCount;

	UPROPERTY(EditAnywhere, Category = "Seeds")
	int strawberrySeedCount;

	UPROPERTY(EditAnywhere, Category = "Seeds")
	int sunflowerSeedCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tools")
	FString equipedToolName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crops")
	FString heldProduceName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Seeds")
	int seedAmount;




	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crops")
	int cropAmount;

	UPROPERTY(EditAnywhere, Category = "Crops")
	int wheatCount;

	UPROPERTY(EditAnywhere, Category = "Crops")
	int cornCount;

	UPROPERTY(EditAnywhere, Category = "Crops")
	int strawberryCount;

	UPROPERTY(EditAnywhere, Category = "Crops")
	int sunflowerCount;

	UPROPERTY(EditAnywhere, Category = "Crops")
	int heldProduceValue;

	int equipedTool = 0;
	int heldSeed = 0;




	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Seeds")
	FString equipedSeedName;

	//Player stats
	UPROPERTY(EditAnywhere, Category = "Character stats")
	int playerStamina = 100;

	void ChangePlayerStam(int amount);
};


