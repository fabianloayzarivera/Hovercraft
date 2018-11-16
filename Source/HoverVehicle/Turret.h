// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurretProjectile.h"
#include "Damageable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Turret.generated.h"

class USelectionWidget;
UCLASS()
class HOVERVEHICLE_API ATurret : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurret();

	UPROPERTY(VisibleAnywhere, Category = "Turret")
		UStaticMeshComponent* TurretBodyMesh;
	UPROPERTY(VisibleAnywhere, Category = "Turret")
		UStaticMeshComponent* TurretHeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<ATurretProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float DetectionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float ProjectileDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float FireCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float Health;
	UPROPERTY(EditAnyWhere, Category = Behavior)
		class UBehaviorTree * BotBehavior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		bool RenderDebugInfo = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<USelectionWidget> SelectionWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
		USelectionWidget* SelectionWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float OffsetWidgetPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		UTexture2D* SelectionImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		UTexture2D* LockedSelectionImage;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void	Tick(float DeltaTime) override;
	virtual void	SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void			RotateToTarget(AActor* Target);
	void			Fire(AActor* Target);
	void			FocusTurret();
	void			UnfocusTurret();
	void			LockTurret();
	void			UnlockTurret();
	virtual void	OnDamage(float value);
	
private:

	
	FTimerHandle		FireCooldownTimerHandle;
	bool				CanFire = true;
	bool				ShowSelectionWidget = false;	
	APawn*				PlayerPawn;
	APlayerController*	PlayerController;
	FVector2D			WidgetScreenPosition;
	void				UpdateSelectionWidget();
	void				CheckifIsInSight();
	void				CanFireAgain();
};
