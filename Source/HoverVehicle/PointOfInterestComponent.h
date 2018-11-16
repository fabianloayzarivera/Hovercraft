// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "PointOfInterestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOVERVEHICLE_API UPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPointOfInterestComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POI")
		TSubclassOf<UUserWidget> PointOfInterestWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "POI")
		UUserWidget* PointOfInterestWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POI")
		float OffsetPosition;


protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector2D	ProjectWorldToScreenBidirectional();
	AActor*		Owner;
	
};
