// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "SelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOVERVEHICLE_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
		void ChangeImage(UTexture2D* Image);

	virtual void RemoveFromParent() override;

	
};
