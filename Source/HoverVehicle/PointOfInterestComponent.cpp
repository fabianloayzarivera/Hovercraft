// Fill out your copyright notice in the Description page of Project Settings.

#include "PointOfInterestComponent.h"
#include "Kismet/GameplayStatics.h"


UPointOfInterestComponent::UPointOfInterestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPointOfInterestComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* aActor = GetOwner();
	if (aActor)
	{
		Owner = aActor;
		if (PointOfInterestWidgetClass)
		{
			PointOfInterestWidget = CreateWidget<UUserWidget>(Owner->GetWorld(), PointOfInterestWidgetClass);
			if (!PointOfInterestWidget)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Widget couldn't be created"));
				return;
			}
			else
			{
				if (!PointOfInterestWidget->IsInViewport())
					PointOfInterestWidget->AddToViewport();

			}
		}
	}
	
}


void UPointOfInterestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PointOfInterestWidget)
	{
		FVector2D ViewportPosition = ProjectWorldToScreenBidirectional();
		PointOfInterestWidget->SetPositionInViewport(ViewportPosition);
	}
}



FVector2D UPointOfInterestComponent::ProjectWorldToScreenBidirectional() 
{
	FVector Projected;
	FVector2D ScreenPosition;
	FVector WorldPosition = Owner->GetActorLocation();
	bool TargetBehindCamera;
	bool PlayerViewportRelative = true;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0);

	ULocalPlayer* const HovercraftPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	if (HovercraftPlayer && HovercraftPlayer->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (HovercraftPlayer->GetProjectionData(HovercraftPlayer->ViewportClient->Viewport, EStereoscopicPass::eSSP_FULL, ProjectionData))
		{
			const FMatrix ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			//const FIntRect ViewRectangle = ProjectionData.GetConstrainedViewRect();
			const FIntRect ViewRectangle = ProjectionData.GetViewRect();

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(WorldPosition, 1.f));
			if (Result.W < 0.f) { TargetBehindCamera = true; }
			if (Result.W == 0.f) { Result.W = 1.f; } // Prevent Divide By Zero

			const float RHW = 1.f / FMath::Abs(Result.W);
			Projected = FVector(Result.X, Result.Y, Result.Z) * RHW;

			// Normalize to 0..1 UI Space
			const float NormX = (Projected.X / 2.f) + 0.5f;
			const float NormY = 1.f - (Projected.Y / 1.0f) - 0.5f;
			//const float NormY = 1.f - (Projected.Y / 2.f) - 0.5f;

			Projected.X = (float)ViewRectangle.Min.X + (NormX * (float)ViewRectangle.Width());
			Projected.Y = (float)ViewRectangle.Min.Y + (NormY * (float)ViewRectangle.Height());

			Projected.X = (OffsetPosition / 2.f) + Projected.X;
			Projected.Y = (OffsetPosition / 2.f) + Projected.Y;

			/* ------------------------------------------ */
			Projected.X = FMath::Clamp<float>(Projected.X, 0, (float)(ViewRectangle.Width() + (int)(OffsetPosition)));
			Projected.Y = FMath::Clamp<float>(Projected.Y, 0, (float)(ViewRectangle.Height() + (int)(OffsetPosition)));

			/* ------------------------------------------ */


			ScreenPosition = FVector2D(Projected.X, Projected.Y);


			if (PlayerViewportRelative)
			{
				ScreenPosition -= FVector2D(ProjectionData.GetConstrainedViewRect().Min);
			}
		}
		else
		{
			ScreenPosition = FVector2D(1234, 5678);
		}
	}

	return ScreenPosition;
}