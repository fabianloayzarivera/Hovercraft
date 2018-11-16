// Fill out your copyright notice in the Description page of Project Settings.

#include "HovercraftProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Damageable.h"
#include "Components/SphereComponent.h"

// Sets default values
AHovercraftProjectile::AHovercraftProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AHovercraftProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bIsHomingProjectile = true;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	ProjectileParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticleSystemComponent"));
	ProjectileParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileDamage = 10.f;

}

// Called when the game starts or when spawned
void AHovercraftProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHovercraftProjectile::Init(float ProjectileSpeed, float _ProjectileDamage, USceneComponent* LockedTarget)
{
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileDamage = _ProjectileDamage;
	if (LockedTarget)
		ProjectileMovement->HomingTargetComponent = LockedTarget;
}

// Called every frame
void AHovercraftProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHovercraftProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() * 2.0f, GetActorLocation());

		FTransform ExplosionTransform;
		ExplosionTransform.SetLocation(this->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, ExplosionTransform);
		IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
		if (DamageableActor)
			DamageableActor->OnDamage(this->ProjectileDamage);
		Destroy();

	}
}

