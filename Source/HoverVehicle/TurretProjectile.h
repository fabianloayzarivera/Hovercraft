// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "TurretProjectile.generated.h"

UCLASS()
class HOVERVEHICLE_API ATurretProjectile : public AActor
{
	GENERATED_BODY()

	float ProjectileDamage;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	
public:

	ATurretProjectile();

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
		UStaticMeshComponent* ProjectileMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		UParticleSystem* ExplosionParticleSystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
		UParticleSystemComponent* ProjectileParticleSystemComponent;
	UFUNCTION(BlueprintCallable)
		void Init(float ProjectileSpeed, float ProjectileDamage);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
	
protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	
	
};
