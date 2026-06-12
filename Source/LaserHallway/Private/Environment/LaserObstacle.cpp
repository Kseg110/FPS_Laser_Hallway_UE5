// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/LaserObstacle.h"
#include "Projectile/Projectile.h"


// Sets default values
ALaserObstacle::ALaserObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetSimulatePhysics(false);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserCollisionComponent"));
	CollisionComponent->SetupAttachment(MeshComponent);
	CollisionComponent->SetBoxExtent(FVector(50.f, 10.f, 10.f));
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1200.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	//ProjectileMovement->Velocity = FVector(0.0f, 1200.0f, 0.0f);
}

// Called when the game starts or when spawned
void ALaserObstacle::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaserObstacle::OnOverlap);
	}

	if (ProjectileMovement)
	{
		FVector ForwardDirection = GetActorForwardVector();
		ProjectileMovement->Velocity = ForwardDirection * ProjectileMovement->InitialSpeed;

		UE_LOG(LogTemp, Warning, TEXT("LaserObstacle: Rotation=%s, Forward %s, Velocity=%s"),
			*GetActorRotation().ToString(),
			*ForwardDirection.ToString(),
			*ProjectileMovement->Velocity.ToString());
	}
	
}

// Called every frame
void ALaserObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserObstacle::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherCompo, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (AProjectile* HitProj = Cast<AProjectile>(OtherActor))
	{
		if (!HitProj->bIsAltProjectile)
		{
			return;
		}
	}

	if (AProjectile* HitProj = Cast<AProjectile>(OtherActor))
	{
		if (!HitProj->bIsAltProjectile)
		{
			return;
		}
	}

	const float DamageAmount = 10.0f;
	UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this, nullptr);

	Destroy(); // delete later if laser is desired to pass through player and not destroy.
}