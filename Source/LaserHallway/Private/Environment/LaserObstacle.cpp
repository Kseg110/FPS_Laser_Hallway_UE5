// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/LaserObstacle.h"


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
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->Velocity = FVector(0.0f, ProjectileMovement->InitialSpeed, 0.0f);
}

// Called when the game starts or when spawned
void ALaserObstacle::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaserObstacle::OnOverlap);
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

	const float DamageAmount = 10.0f;
	UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this, nullptr);

	Destroy(); // delete later if laser is desired to pass through player and not destroy.
}