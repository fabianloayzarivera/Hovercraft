// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HoverVehicleGameMode.h"
#include "HoverVehicleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHoverVehicleGameMode::AHoverVehicleGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Hovercraft/Blueprints/BP_Hovecraft.BP_Hovecraft"));

	/*if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}
