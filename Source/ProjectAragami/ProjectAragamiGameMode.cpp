// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectAragamiGameMode.h"
#include "ProjectAragamiHUD.h"
#include "ProjectAragamiCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectAragamiGameMode::AProjectAragamiGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProjectAragamiHUD::StaticClass();
}
