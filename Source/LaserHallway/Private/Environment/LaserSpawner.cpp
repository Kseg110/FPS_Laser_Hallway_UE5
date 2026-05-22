// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Environment/LaserSpawner.h"
#include "Environment/LaserObstacle.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALaserSpawner::ALaserSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = SpawnVolume;
	SpawnVolume->SetBoxExtent(FVector(200.f, 50.f, 50.f));
	SpawnVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ALaserSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!LaserObstacleClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALaserSpawner: LaserObstacleClass not set on %s"), *GetName());
		return;
	}
	
	if (bStartImmediate)
	{
		SpawnLaser();
	}

	if (SpawnInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ALaserSpawner::SpawnLaser, SpawnInterval, true);
	}
}

// Called every frame
//void ALaserSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ALaserSpawner::SpawnLaser()
{
	if (!GetWorld() || !LaserObstacleClass)
	{
		return;
	}

	const FVector Origin = SpawnVolume->GetComponentLocation();
	const FVector Extent = SpawnVolume->GetScaledBoxExtent();
	const FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	const FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ALaserObstacle* Spawned = GetWorld()->SpawnActor<ALaserObstacle>(LaserObstacleClass, SpawnLocation, SpawnRotation, Params);
	if (!Spawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALaserSpawner: Failed to spawn laser form %s"), *GetName());
	}
}

