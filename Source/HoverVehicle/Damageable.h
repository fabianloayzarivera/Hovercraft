// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOVERVEHICLE_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void OnDamage(float value) = 0;
	
};
