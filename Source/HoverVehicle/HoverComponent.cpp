// Fill out your copyright notice in the Description page of Project Settings.

#include "HoverComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Hovercraft.h"


UHoverComponent::UHoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SpringCoefficient = 400.0f;
	HoverDistance = 100.0f;
	DampingCoefficient = 80.0f;
	PreviousHoverDelta = 0.0f;
	HoverDelta = 0.0f;
	// ...
}

void UHoverComponent::Init(AActor* aActor) 
{
	if (aActor)
	{
		AHovercraft* Parent = Cast<AHovercraft>(aActor);
		if (Parent)
		{
			Owner = Parent;
			OwnerMesh = Cast<UPrimitiveComponent>(Owner->GetMeshComponent());
			OwnerMesh->SetLinearDamping(3.f);
			OwnerMesh->SetAngularDamping(5.f);
		}
	}
}

AActor* UHoverComponent::GetOwner() const 
{
	return Owner;
}


void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Owner)
	{
		FVector StartLocation = this->GetComponentLocation();
		FVector EndLocation = StartLocation - (this->GetUpVector() * HoverDistance);
		FHitResult TraceResult;
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceAsyncScene = true;
		TraceParams.AddIgnoredActor(Owner);
		GetWorld()->LineTraceSingleByChannel(TraceResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		if (TraceResult.bBlockingHit)
		{
			float Spring = 0.0f;
			float Damper = 0.0f;
			FVector Force;

			if (RenderDebugInfo)
			{
				DrawDebugLine(GetWorld(), StartLocation, TraceResult.ImpactPoint, FColor::Green, false, 0.01f);
				DrawDebugPoint(GetWorld(), TraceResult.ImpactPoint, 5.0f, FColor::Blue, false, 0.01f);
			}
			HoverDelta = HoverDistance - TraceResult.Distance;
			Spring = HoverDelta * SpringCoefficient;
			Damper = ((HoverDelta - PreviousHoverDelta) / UGameplayStatics::GetWorldDeltaSeconds(this)) * DampingCoefficient;
			Force = this->GetUpVector() * (Damper + Spring);
			OwnerMesh->AddForceAtLocation(Force, this->GetComponentLocation());
			
			PreviousHoverDelta = HoverDelta;
		}
		else 
		{
			if (RenderDebugInfo)
			{
				DrawDebugLine(GetWorld(), StartLocation, TraceResult.ImpactPoint, FColor::Red, false, 0.01f);
			}
		}
	}

}

