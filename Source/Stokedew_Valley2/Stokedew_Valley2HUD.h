// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Stokedew_Valley2HUD.generated.h"

UCLASS()
class AStokedew_Valley2HUD : public AHUD
{
	GENERATED_BODY()

public:
	AStokedew_Valley2HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

