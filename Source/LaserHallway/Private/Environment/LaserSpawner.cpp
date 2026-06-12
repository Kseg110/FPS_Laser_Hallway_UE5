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

	// Setup Default pattern
	FLaserPatternConfig DefaultPattern;
	DefaultPattern.PatternType = ELaserPattern::Random;
	DefaultPattern.Count = 1;
	AvailablePatterns.Add(DefaultPattern);

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

	if (AvailablePatterns.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALaserSpawner: No Patterns configured on %s"), *GetName());
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
void ALaserSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserSpawner::SpawnLaser()
{
	if (!GetWorld() || !LaserObstacleClass || AvailablePatterns.Num() == 0)
	{
		return;
	}

	// Select Pattern
	const FLaserPatternConfig* SelectedPattern = nullptr;
	if (bRandomizePatternSelection)
	{
		int32 RandomIndex = FMath::RandRange(0, AvailablePatterns.Num() - 1);
		SelectedPattern = &AvailablePatterns[RandomIndex];
	}
	else
	{
		SelectedPattern = &AvailablePatterns[CurrentPatternIndex];
		CurrentPatternIndex = (CurrentPatternIndex + 1) % AvailablePatterns.Num();
	}
	SpawnPattern(*SelectedPattern);

	// Old logic before Enumerated patterns
	/*const FVector Origin = SpawnVolume->GetComponentLocation();
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
	}*/
}

void ALaserSpawner::SpawnPattern(const FLaserPatternConfig& Pattern)
{
	TArray<FVector> SpawnPositions = GeneratePatternPositions(Pattern);

	// Determine rotation based on orientation
	FRotator BaseRotation = GetActorRotation();
	FQuat RotationQuat = FQuat::Identity;

	if (Pattern.Orientation == ELaserOrientation::Vertical)
	{
		BaseRotation.Roll += 90.0f; // rotates laser to be Vertical
		RotationQuat = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(90.0f));
	}

	FVector SpawnCenter = SpawnVolume->GetComponentLocation();

	for (int32 i = 0; i < SpawnPositions.Num(); ++i)
	{
		FVector& LocalPos = SpawnPositions[i];

		if (Pattern.Orientation == ELaserOrientation::Vertical)
		{
			LocalPos = RotationQuat.RotateVector(LocalPos);
		}

		FVector WorldPos = SpawnCenter + LocalPos;
		SpawnSingleLaser(WorldPos, BaseRotation);
	}
}

TArray<FVector> ALaserSpawner::GeneratePatternPositions(const FLaserPatternConfig& Pattern)
{
	TArray<FVector> Positions;
	const FVector Origin = FVector::ZeroVector; // For local Space

	switch (Pattern.PatternType)
	{
	case ELaserPattern::Random:
		{
		const FVector Extent = SpawnVolume->GetScaledBoxExtent();
		FVector RandomPos = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
		RandomPos.Z += Pattern.HeighOffset;
		Positions.Add(RandomPos);
		}
	break;

	case ELaserPattern::HorizontalRow:
		{
		float TotalWidth = (Pattern.Count - 1) * Pattern.Spacing;
		float StartX = -TotalWidth / 2.0f;

		for (int32 i = 0; i < Pattern.Count; ++i)
		{
			FVector Pos = Origin;
			Pos.X = StartX + (i * Pattern.Spacing);
			Pos.Z = Pattern.HeighOffset;

			if (Pattern.bRandomizePositions)
			{
				Pos.X += FMath::RandRange(-Pattern.RandomOffsetRange, Pattern.RandomOffsetRange);
				Pos.Z += FMath::RandRange(-Pattern.RandomOffsetRange, Pattern.RandomOffsetRange);
			}
			Positions.Add(Pos);
		}
		}
		break;

	case ELaserPattern::VerticalColumn:
		{
		float TotalHeight = (Pattern.Count - 1) * Pattern.Spacing;
		float StartZ = -TotalHeight / 2.0f;

		for (int32 i = 0; i < Pattern.Count; ++i)
		{
			FVector Pos = Origin;
			Pos.Z = StartZ + (i * Pattern.Spacing) + Pattern.HeighOffset;

			if (Pattern.bRandomizePositions)
			{
				Pos.X += FMath::RandRange(-Pattern.RandomOffsetRange, Pattern.RandomOffsetRange);
				Pos.Z += FMath::RandRange(-Pattern.RandomOffsetRange, Pattern.RandomOffsetRange);
			}
			Positions.Add(Pos);
		}
		}
		break;

	case ELaserPattern::Grid:
		{
		float TotalWidth = (Pattern.Count - 1) * Pattern.Spacing;
		float TotalHeight = (Pattern.SecondaryCount - 1) * Pattern.Spacing;
		float StartX = -TotalWidth / 2.0f;
		float StartZ = -TotalHeight / 2.0f;

		for (int32 Row = 0; Row < Pattern.SecondaryCount; ++Row)
		{
			for (int32 Col = 0; Col < Pattern.Count; ++Col)
			{
				FVector Pos = Origin;
				Pos.X = StartX + (Col * Pattern.Spacing);
				Pos.Z = StartZ + (Row * Pattern.Spacing) + Pattern.HeighOffset;

				if (Pattern.bRandomizePositions)
				{
					Pos.X += FMath::RandRange(-Pattern.RandomOffsetRange, Pattern.RandomOffsetRange);
					Pos.Z += FMath::RandRange(-Pattern.RandomOffsetRange, Pattern.RandomOffsetRange);
				}
				Positions.Add(Pos);
			}
		}
		}
		break;

	case ELaserPattern::Custom:
		{
		for (FVector CustomPos : Pattern.CustomPositions)
		{
			CustomPos.Z += Pattern.HeighOffset;
			Positions.Add(CustomPos);
		}
		}
		break;
	}
	return Positions;
}

void ALaserSpawner::SpawnSingleLaser(const FVector& Location, const FRotator& Rotation)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UE_LOG(LogTemp, Warning, TEXT("ALaserSpawner: Attempting to spawn laser with Rotatin: %s"), *Rotation.ToString());

	ALaserObstacle* Spawned = GetWorld()->SpawnActor<ALaserObstacle>(LaserObstacleClass, Location, Rotation, Params);
	if (!Spawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALaserSpawner: Failed to spawn laser from %s"), *GetName());
	}
}

