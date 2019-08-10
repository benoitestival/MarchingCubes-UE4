// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesGameMode.h"
#include "MarchingCubesHUD.h"
#include "MarchingCubesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMarchingCubesGameMode::AMarchingCubesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMarchingCubesHUD::StaticClass();
}
