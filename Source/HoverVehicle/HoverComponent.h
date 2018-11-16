// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HoverComponent.generated.h"

class AHovercraft;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOVERVEHICLE_API UHoverComponent : public USceneComponent
{
	GENERATED_BODY()

	AHovercraft* Owner;
public:	
	// Sets default values for this component's properties
	UHoverComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverComponent")
		float HoverDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverComponent")
		float SpringCoefficient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverComponent")
		float DampingCoefficient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverComponent")
		bool RenderDebugInfo = false;

	void	Init(AActor* aActor);
	AActor* GetOwner() const;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float				 PreviousHoverDelta;
	float				 HoverDelta;
	UPrimitiveComponent* OwnerMesh;
	
};
