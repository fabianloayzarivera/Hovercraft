// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HoverVehicleCharacter.generated.h"

// THIS CLASS IS NOT BEING USED, IT CAME WITH THE UE4 TEMPLATE!! //w

UCLASS(config=Game)
class AHoverVehicleCharacter : public ACharacter
{
	GENERATED_BODY()

	
public:
	AHoverVehicleCharacter();


protected:


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:

};

