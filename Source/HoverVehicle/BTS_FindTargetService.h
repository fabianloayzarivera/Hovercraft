// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindTargetService.generated.h"

/**
 * 
 */
UCLASS()
class HOVERVEHICLE_API UBTS_FindTargetService : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_FindTargetService();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
