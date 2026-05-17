// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FusionCore.h"
#include "Projectile/Projectile.h"
#include "HUD/LaserHUD.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFusionCore::AFusionCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and make collision componenet the root of actor
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetGenerateOverlapEvents(false);

	// Create visual mesh and attach to root
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoreMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void AFusionCore::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	RunningTime = 0.0f;

	// Bind Hit
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &AFusionCore::OnHit);
	}
}

// Called every frame
void AFusionCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Bobbing on Z-Axis
	RunningTime += DeltaTime;
	const float DeltaZ = BobbingAmplitude * FMath::Sin(RunningTime * BobbingSpeed);
	FVector NewLocation = StartLocation;
	NewLocation.Z += DeltaZ;
	SetActorLocation(NewLocation);
}

void AFusionCore::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Projectile Hits, Add score and destroy Fusion Core
	if (OtherActor && OtherActor != this)
	{
		if (AProjectile* HitProjectile = Cast<AProjectile>(OtherActor))
		{
			HitProjectile->Destroy();

			// add score to player HUD
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (ALaserHUD* HUD = Cast<ALaserHUD>(PC->GetHUD()))
				{
					HUD->AddScore(100);
				}
			}
			Destroy();
		}
	}
}

