// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_FindTargetService.h"
#include "AIC_Turret.h"
#include "Turret.h"
#include "Hovercraft.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTS_FindTargetService::UBTS_FindTargetService()
{
	bCreateNodeInstance = true;
}

void UBTS_FindTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIC_Turret * TurretAi = Cast<AAIC_Turret>(OwnerComp.GetAIOwner());

	if (TurretAi)
	{
		ATurret * TurretPawn = Cast<ATurret>(TurretAi->GetPawn());

		if (TurretPawn)
		{

			AHovercraft* Hovercraft = Cast<AHovercraft>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			FVector DistanceVector = Hovercraft->GetActorLocation() - TurretPawn->GetActorLocation();
			float Distance = DistanceVector.Size();
			if (Distance <= TurretPawn->DetectionRadius)
			{
				TurretAi->SetTarget(Hovercraft);
				TurretAi->SetSeenTarget(true);
			}
			else
			{
				TurretAi->SetTarget(nullptr);
				TurretAi->SetSeenTarget(false);
				

			}
			AActor * TargetActor = TurretAi->GetTarget();

		}
	}
}


