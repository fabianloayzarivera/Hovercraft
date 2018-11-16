// Fill out your copyright notice in the Description page of Project Settings.

#include "Hovercraft.h"
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HoverComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HovercraftProjectile.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Turret.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AHovercraft::AHovercraft()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HovercraftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HovercraftMesh"));
	RootComponent = HovercraftMesh;

	HovercraftParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HovercraftParticleSystem"));
	HovercraftParticleSystem->SetupAttachment(RootComponent);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HoverComponent1 = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponent1"));
	HoverComponent2 = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponent2"));
	HoverComponent3 = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponent3"));
	HoverComponent4 = CreateDefaultSubobject<UHoverComponent>(TEXT("HoverComponent4"));

	HoverComponent1->SetupAttachment(RootComponent);
	HoverComponent2->SetupAttachment(RootComponent);
	HoverComponent3->SetupAttachment(RootComponent);
	HoverComponent4->SetupAttachment(RootComponent);

	ForwardForce = 50000.f;
	TurnTorque = 10000000.F;

}

// Called when the game starts or when spawned
void AHovercraft::BeginPlay()
{
	Super::BeginPlay();
	HoverComponent1->Init(this);
	HoverComponent2->Init(this);
	HoverComponent3->Init(this);
	HoverComponent4->Init(this);
	
}

// Called every frame
void AHovercraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentMode)
	{
	case EMode::EM_NORMAL_MODE:
		break;
	case EMode::EM_FIGHT_MODE:
		for (auto It = TurretsInSight.CreateIterator(); It; It++)
		{
			FVector LookAtVector = (*It)->GetActorLocation() - this->GetActorLocation();
			LookAtVector.Normalize();
			float DotProduct = FVector::DotProduct(this->GetActorForwardVector(), LookAtVector);
			if ((DotProduct > 0.99f) && ((*It) != TargetTurret))
			{
				if(TargetTurret)
					TargetTurret->UnfocusTurret();
				TargetTurret = (*It);
			}
		}
		if (TargetTurret)
			TargetTurret->FocusTurret();
		
		break;
	case EMode::EM_LOCKED_MODE:
		break;
	default:
		break;
	}
	
}

// Called to bind functionality to input
void AHovercraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AHovercraft::MoveForward);
	PlayerInputComponent->BindAxis("TurnRight", this, &AHovercraft::TurnRight);
	PlayerInputComponent->BindAction("FightModeSwitch", IE_Pressed, this, &AHovercraft::SwitchFightMode);
	PlayerInputComponent->BindAction("FlipFlopLockTarget", IE_Pressed, this, &AHovercraft::FlipFlopLockTarget);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHovercraft::Fire);

}

void AHovercraft::MoveForward(float value) 
{
	FowardAxisValue = value;
	FVector Force;
	Force = (HovercraftMesh->GetForwardVector() * ForwardForce) * value;
	HovercraftMesh->AddForce(Force);
}


void AHovercraft::TurnRight(float value)
{
	FVector Torque;
	
	
	//float DotProduct = FVector::DotProduct(HovercraftMesh->GetPhysicsLinearVelocity(), HovercraftMesh->GetForwardVector());
	//Are we moving forward or backwards? We can use Dot product aswell
	if (FowardAxisValue < 0)
		Torque = HovercraftMesh->GetUpVector() * (TurnTorque * value * -1);
	else
		Torque = HovercraftMesh->GetUpVector() * (TurnTorque * value);

	
	HovercraftMesh->AddTorque(Torque);
}

UStaticMeshComponent* AHovercraft::GetMeshComponent() const 
{
	return HovercraftMesh;
}

void AHovercraft::AddTurretInSight(ATurret* TurretPawn) 
{
	if (!TurretsInSight.Contains(TurretPawn))
		TurretsInSight.Add(TurretPawn);

}

void AHovercraft::RemoveTurretInSight(ATurret* TurretPawn)
{
	int32 index = -1; 
	TurretsInSight.Find(TurretPawn, index);
	if (index >= 0)
	{
		TurretsInSight.RemoveAt(index);
		if ((TurretPawn == TargetTurret) && (TargetTurret) && (TurretPawn != LockedTurret))
		{
			TargetTurret->UnfocusTurret();
			TargetTurret = nullptr;
		}
	}
}

void AHovercraft::SwitchFightMode() 
{

	if (CurrentMode == EMode::EM_FIGHT_MODE)
	{
		CurrentMode = EMode::EM_NORMAL_MODE;
		OnHovercraftModeChanged.Broadcast(CurrentMode);
		if (TargetTurret)
			TargetTurret->UnfocusTurret();
		TargetTurret = nullptr;
	}
	else if (CurrentMode == EMode::EM_LOCKED_MODE)
	{
		CurrentMode = EMode::EM_NORMAL_MODE;
		OnHovercraftModeChanged.Broadcast(CurrentMode);
		if(LockedTurret)
			LockedTurret->UnlockTurret();
		if (TargetTurret)
			TargetTurret->UnfocusTurret();
		TargetTurret = nullptr;
		LockedTurret = nullptr;
	}
	else
	{
		CurrentMode = EMode::EM_FIGHT_MODE;
		OnHovercraftModeChanged.Broadcast(CurrentMode);

	}
}

void AHovercraft::FlipFlopLockTarget()
{
	if (!LockedTurret)
	{
		if (TargetTurret)
		{
			LockedTurret = TargetTurret;
			LockedTurret->LockTurret();
			CurrentMode = EMode::EM_LOCKED_MODE;
			OnHovercraftModeChanged.Broadcast(CurrentMode);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("No Target To Lock!")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Unlocked Target!")));
		LockedTurret->UnlockTurret();
		LockedTurret = nullptr;
		if (TargetTurret)
			TargetTurret->UnfocusTurret();
		TargetTurret = nullptr;
		CurrentMode = EMode::EM_FIGHT_MODE;
		OnHovercraftModeChanged.Broadcast(CurrentMode);
	}

}

void AHovercraft::Fire() 
{
	if (LockedTurret)
	{
		if (CanFire)
		{
			
			if (ProjectileClass)
			{
				CanFire = false;
				GetWorld()->GetTimerManager().SetTimer(FireBurstTimerHandle, this, &AHovercraft::FireSimpleShot, BurstDelay, true, 0.01f);
				GetWorld()->GetTimerManager().SetTimer(FireCooldownTimerHandle, this, &AHovercraft::CanFireAgain, FireCooldown, false);
			}
			else 
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("No Projectile Class set!")));

		}
		else 
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Weapon in cooldown!")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("No locked target to fire at!")));
	}
}

void AHovercraft::FireSimpleShot() 
{
	FVector StartLocation = this->HovercraftMesh->GetSocketLocation("FireSocket");
	FTransform ProjectileTransform = FTransform::Identity;
	FVector ProjectileRotation = this->GetActorForwardVector();
	ProjectileTransform.SetLocation(StartLocation);
	ProjectileTransform.SetRotation(ProjectileRotation.Rotation().Quaternion());
	AHovercraftProjectile* ProjectileShot = GetWorld()->SpawnActorDeferred<AHovercraftProjectile>(ProjectileClass, FTransform::Identity, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if(ProjectileShot && LockedTurret)
	{
		ProjectileShot->Init(this->ProjectileSpeed, this->ProjectileDamage, LockedTurret->TurretBodyMesh);
		UGameplayStatics::FinishSpawningActor(ProjectileShot, ProjectileTransform);
		ShotCounter++;
	}	
	if (ShotCounter >= ProjectilesPerShot)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireBurstTimerHandle);
		FireBurstTimerHandle.Invalidate();
		ShotCounter = 0;
	}
}

void AHovercraft::CanFireAgain() 
{
	CanFire = true;
}

void AHovercraft::TurretDestroyed(ATurret* TurretPawn) 
{
	if (TurretPawn) 
	{
		if (TurretPawn == LockedTurret)
		{
			if (TargetTurret)
				TargetTurret->UnfocusTurret();
			if(LockedTurret)
				LockedTurret->UnlockTurret();
			LockedTurret = nullptr;
			TargetTurret = nullptr;
			TurretsDestroyed++;
			OnHovercraftTurretDestroyed.Broadcast(TurretsDestroyed);
			CurrentMode = EMode::EM_FIGHT_MODE;
			OnHovercraftModeChanged.Broadcast(CurrentMode);
		}
	}
}

void AHovercraft::OnDamage(float value)
{
	this->Health -= value;
	OnHovercraftReceivedDamage.Broadcast(Health);
	if (Health <= 0)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
			PlayerController->SetPause(true);
	}

}