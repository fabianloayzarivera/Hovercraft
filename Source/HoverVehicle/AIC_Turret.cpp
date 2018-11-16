// Fill out your copyright notice in the Description page of Project Settings.

#include "AIC_Turret.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Hovercraft.h"
#include "Turret.h"

AAIC_Turret::AAIC_Turret(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BrainComponent = BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

}

void AAIC_Turret::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ATurret* Turret = Cast<ATurret>(InPawn);
	if (Turret && Turret->BotBehavior) 
	{
		BlackboardComp->InitializeBlackboard(*Turret->BotBehavior->BlackboardAsset);
		TargetId = BlackboardComp->GetKeyID("TargetObject");
		BehaviorComp->StartTree(*Turret->BotBehavior);
	}

}

void AAIC_Turret::SetTarget(AActor* Target) 
{

	TargetObject = Target;
	BlackboardComp->SetValue<UBlackboardKeyType_Object>(TargetId, Target);
	SetFocus(TargetObject);
}

AActor* AAIC_Turret::GetTarget() const
{
	return TargetObject;
}

void AAIC_Turret::SetSeenTarget(bool SeenTarget)
{
	TargetSeen = SeenTarget;
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(BKTargetObject, TargetSeen);
	}
}


void AAIC_Turret::BeginPlay()
{
	Super::BeginPlay();
}



