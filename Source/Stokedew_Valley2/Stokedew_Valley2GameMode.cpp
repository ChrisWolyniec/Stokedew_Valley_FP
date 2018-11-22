// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Stokedew_Valley2GameMode.h"
#include "Stokedew_Valley2HUD.h"
#include "Stokedew_Valley2Character.h"
#include "UObject/ConstructorHelpers.h"

AStokedew_Valley2GameMode::AStokedew_Valley2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AStokedew_Valley2HUD::StaticClass();
}
