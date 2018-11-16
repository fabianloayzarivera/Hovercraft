// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Damageable.h"
#include "Hovercraft.generated.h"


UENUM(BlueprintType)
enum class EMode : uint8
{
	EM_NORMAL_MODE UMETA(DisplaName = "Normal Mode"),
	EM_FIGHT_MODE  UMETA(DisplaName = "Fight Mode"),
	EM_LOCKED_MODE UMETA(DisplaName = "Locked Mode")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHovercraftDamageReceived, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHovercraftTurretDestroyed, int32, TurretsDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHovercraftModeChanged, EMode, CurrentMode);
class UHoverComponent;
class AHovercraftProjectile;
class ATurret;
UCLASS()
class HOVERVEHICLE_API AHovercraft : public APawn, public IDamageable
{
	GENERATED_BODY()

public:

	AHovercraft();


	UPROPERTY(VisibleAnywhere, Category = "HoverCraft")
		UStaticMeshComponent* HovercraftMesh;
	UPROPERTY(VisibleAnywhere, Category = "HoverCraft")
		UParticleSystemComponent* HovercraftParticleSystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float ForwardForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float TurnTorque;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		TSubclassOf<AHovercraftProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float ProjectileDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float BurstDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float FireCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		int32 ProjectilesPerShot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		UHoverComponent* HoverComponent1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		UHoverComponent* HoverComponent2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		UHoverComponent* HoverComponent3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HoverCraft")
		UHoverComponent* HoverComponent4;
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
		FHovercraftDamageReceived OnHovercraftReceivedDamage;
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
		FHovercraftTurretDestroyed OnHovercraftTurretDestroyed;
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
		FHovercraftModeChanged OnHovercraftModeChanged;

	UFUNCTION()
		void AddTurretInSight(ATurret* TurretPawn);
	UFUNCTION()
		void RemoveTurretInSight(ATurret* TurretPawn);
	UFUNCTION()
		void Fire();
	UFUNCTION()
		void TurretDestroyed(ATurret* TurretPawn);

	virtual void			OnDamage(float value);
	virtual void			Tick(float DeltaTime) override;
	virtual void			SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void					MoveForward(float value);
	void					TurnRight(float value);
	UStaticMeshComponent*	GetMeshComponent() const;

protected:

	virtual void BeginPlay() override;

private:
	EMode				CurrentMode = EMode::EM_NORMAL_MODE;
	float				FowardAxisValue;
	TArray<ATurret*>	TurretsInSight;	
	ATurret*			TargetTurret = nullptr;
	ATurret*			LockedTurret = nullptr;	
	FTimerHandle		FireCooldownTimerHandle;
	FTimerHandle		FireBurstTimerHandle;	
	int32				TurretsDestroyed = 0;
	int32				ShotCounter = 0;

	bool CanFire = true;
	void CanFireAgain();
	void FireSimpleShot();
	void SwitchFightMode();
	void FlipFlopLockTarget();
};
