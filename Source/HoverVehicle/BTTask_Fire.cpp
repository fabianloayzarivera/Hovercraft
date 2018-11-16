// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Fire.h"
#include "Turret.h"
#include "AIC_Turret.h"


UBTTask_Fire::UBTTask_Fire()
{
	NodeName = "RotateToTargetTask";
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIC_Turret * TurretAi = Cast<AAIC_Turret>(OwnerComp.GetAIOwner());
	if (TurretAi && TurretAi->GetTarget())
	{
		ATurret * TurretPawn = Cast<ATurret>(TurretAi->GetPawn());
		TurretPawn->Fire(TurretAi->GetTarget());

		Result = EBTNodeResult::Succeeded;
	}

	return Result;
}

