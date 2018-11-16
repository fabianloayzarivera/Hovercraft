// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Turret.generated.h"

/**
 * 
 */
UCLASS()
class HOVERVEHICLE_API AAIC_Turret : public AAIController
{
	GENERATED_BODY()
	
	FName BKTargetObject = "TargetObject";
public:
	AAIC_Turret(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
		AActor* GetTarget() const;
	UFUNCTION(BlueprintCallable)
		void SetTarget(AActor * Target);

	void SetSeenTarget(bool SeenTarget);
	void Possess(APawn *InPawn);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(transient)
		class UBlackboardComponent * BlackboardComp;
	UPROPERTY(transient)
		class UBehaviorTreeComponent * BehaviorComp;

	AActor* TargetObject;
	bool	TargetSeen = false;
	int32	TargetId;
};
