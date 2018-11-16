// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Hovercraft.h"
#include "SelectionWidget.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATurret::ATurret()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComp"));
	RootComponent = DefaultSceneComponent;

	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBodyMesh"));
	TurretBodyMesh->SetupAttachment(RootComponent);

	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHeadMesh"));
	TurretHeadMesh->SetupAttachment(TurretBodyMesh);

	DetectionRadius = 400.0f;
	ProjectileDamage = 15.0f;
	ProjectileSpeed = 3000.0f;
	FireCooldown = 1.f;
	Health = 100.f;
}


void ATurret::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
		PlayerPawn = PlayerController->GetPawn();
	if (SelectionWidgetClass)
	{
		SelectionWidget = CreateWidget<USelectionWidget>(GetWorld(), SelectionWidgetClass);
		if (!SelectionWidget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Widget couldn't be created"));
			return;
		}

	}

}


void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckifIsInSight();

	if (ShowSelectionWidget)
	{
		if (!SelectionWidget->IsInViewport())
			SelectionWidget->AddToViewport();

		UpdateSelectionWidget();
	}
	else 
	{
		if(SelectionWidget)
			SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	/*----------------------------------------------*/
	
}

void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurret::RotateToTarget(AActor* Target) 
{
	if (Target) 
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TurretHeadMesh->GetComponentLocation(), Target->GetActorLocation());
		TurretHeadMesh->SetWorldRotation(LookAtRotation);
	}
}

void ATurret::Fire(AActor* Target)
{
	if (Target)
	{
		if (CanFire)
		{

			FVector StartLocation = this->TurretHeadMesh->GetSocketLocation("FireSocket");
			FVector EndLocation = Target->GetActorLocation();
			FHitResult TraceResult;
			FCollisionQueryParams TraceParams;
			TraceParams.bTraceAsyncScene = true;
			TraceParams.AddIgnoredActor(this);
			GetWorld()->LineTraceSingleByChannel(TraceResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

			if (TraceResult.bBlockingHit && TraceResult.Actor == Target)
			{	
				AHovercraft* HoverCraft = Cast<AHovercraft>(Target);
				FVector PredictedPosition;
				float Distance = (HoverCraft->GetActorLocation() - StartLocation).Size();
				float Time = Distance / ProjectileSpeed;
				PredictedPosition = HoverCraft->GetActorLocation() + (HoverCraft->GetMeshComponent()->GetPhysicsLinearVelocity() * Time);

				FVector SpawnVector = PredictedPosition - StartLocation;
				SpawnVector.Normalize();

				FTransform ProjectileTransform = FTransform::Identity;
				ProjectileTransform.SetLocation(StartLocation);
				ProjectileTransform.SetRotation(SpawnVector.Rotation().Quaternion());
				
				ATurretProjectile* ProjectileShot = GetWorld()->SpawnActorDeferred<ATurretProjectile>(ProjectileClass, FTransform::Identity, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				ProjectileShot->Init(this->ProjectileSpeed, this->ProjectileDamage);
				UGameplayStatics::FinishSpawningActor(ProjectileShot, ProjectileTransform);

				CanFire = false;
				GetWorld()->GetTimerManager().SetTimer(FireCooldownTimerHandle, this, &ATurret::CanFireAgain, FireCooldown, false);

				if (RenderDebugInfo)
				{
					DrawDebugLine(GetWorld(), StartLocation, TraceResult.ImpactPoint, FColor::Green, false, 0.5f);
					DrawDebugPoint(GetWorld(), TraceResult.ImpactPoint, 5.0f, FColor::Blue, false, 0.5f);

					DrawDebugLine(GetWorld(), StartLocation, PredictedPosition, FColor::Blue, false, 0.5f);
					DrawDebugPoint(GetWorld(), PredictedPosition, 10.0f, FColor::Green, false, 0.5f);
				}

				
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
}

void ATurret::CanFireAgain() 
{
	CanFire = true;
}

void ATurret::UpdateSelectionWidget()
{
	if (SelectionWidget)
	{
		SelectionWidget->SetVisibility(ESlateVisibility::Visible);
		SelectionWidget->SetPositionInViewport(WidgetScreenPosition, true);

	}

}

void ATurret::CheckifIsInSight() 
{
	
	int ViewportX, ViewportY = 0.f;
		
	PlayerController->GetViewportSize(ViewportX, ViewportY);
	PlayerController->ProjectWorldLocationToScreen(this->GetActorLocation(), WidgetScreenPosition);
	WidgetScreenPosition.X += OffsetWidgetPosition / 2.f;
	WidgetScreenPosition.Y += OffsetWidgetPosition / 2.f;

	if ((WidgetScreenPosition.X < 0.f || WidgetScreenPosition.X > ViewportX) || (WidgetScreenPosition.Y < 0.f || WidgetScreenPosition.Y > ViewportY) && ShowSelectionWidget)
	{
		/*Turret out of sight*/
		WidgetScreenPosition = FVector2D(-1.f, -1.f);
		AHovercraft* Hovercraft = Cast<AHovercraft>(PlayerPawn);
		Hovercraft->RemoveTurretInSight(this);
	}
	else if ((WidgetScreenPosition.X > 0.f && WidgetScreenPosition.X < ViewportX) && (WidgetScreenPosition.Y > 0.f && WidgetScreenPosition.Y < ViewportY) && !ShowSelectionWidget)
	{
		/*Turret on sight*/
		AHovercraft* Hovercraft = Cast<AHovercraft>(PlayerPawn);
		Hovercraft->AddTurretInSight(this);
	}

}

void ATurret::OnDamage(float value)
{
	this->Health -= value;
	if (Health <= 0)
	{
		AHovercraft* Hovercraft = Cast<AHovercraft>(PlayerPawn);
		Hovercraft->TurretDestroyed(this);
		Hovercraft->RemoveTurretInSight(this);
		if (SelectionWidget)
			SelectionWidget->RemoveFromParent();
		Destroy();
	}

}

void ATurret::FocusTurret() 
{
	if (!ShowSelectionWidget)
		ShowSelectionWidget = true;
}

void ATurret::UnfocusTurret() 
{
	if (ShowSelectionWidget)
		ShowSelectionWidget = false;
}

void ATurret::LockTurret() 
{
	SelectionWidget->ChangeImage(LockedSelectionImage);
}

void ATurret::UnlockTurret() 
{
	SelectionWidget->ChangeImage(SelectionImage);
}

