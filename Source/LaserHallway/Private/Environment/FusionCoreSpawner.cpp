// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Environment/FusionCoreSpawner.h"
#include "Environment/FusionCore.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFusionCoreSpawner::AFusionCoreSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = SpawnVolume;
	SpawnVolume->SetBoxExtent(FVector(200.f, 50.f, 50.f));
	SpawnVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AFusionCoreSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!FusionCoreClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFusionCoreSpawner: FusionCoreClass not set on %s"), * GetName());
			return;
	}

	if (bStartImmediate)
	{
		SpawnFusionCore();
	}

	if (SpawnInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AFusionCoreSpawner::SpawnFusionCore, SpawnInterval, true);
	}
	
}

// Called every frame
void AFusionCoreSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFusionCoreSpawner::SpawnFusionCore()
{
	const FVector Origin = SpawnVolume->GetComponentLocation();
	const FVector Extent = SpawnVolume->GetScaledBoxExtent();
	const FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	const FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AFusionCore* Spawned = GetWorld()->SpawnActor<AFusionCore>(FusionCoreClass, SpawnLocation, SpawnRotation, Params);
	if (!Spawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFusionCoreSpawner: Failed to spawn fusion core %s"), *GetName());
	}
}

