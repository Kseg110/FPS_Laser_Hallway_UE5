// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserSpawner.generated.h"

class UBoxComponent;
class ALaserObstacle;

UENUM(BlueprintType)
enum class ELaserOrientation : uint8
{
	Horizontal UMETA(DisplayName = "Horizontal (Default)"),
	Vertical UMETA(DisplayName = "Vertical (Rotate 90 Degrees)")
};

UENUM(BlueprintType)
enum class ELaserPattern : uint8
{
	Random UMETA(DisplayName = "Random Single"),
	HorizontalRow UMETA(DiplayName = "Horizontal Row"),
	VerticalColumn UMETA(DisplayName = "Vertical Column"),
	Grid UMETA(DisplayName = "Grid Pattern"),
	Custom UMETA(DisplayName = "Custom Positions")
};

USTRUCT(BlueprintType)
struct FLaserPatternConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	ELaserPattern PatternType = ELaserPattern::Random;

	//HorizontalRow: for the number of laser across (X-axis)
	//VerticalColumn: for number of lasers vertically (Y-axis)
	//Grid count in  primary axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (ClampMin = "1", ClampMax = "20"))
	int32 Count = 1;
	
	// GRID: count in secondary axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (ClampMin = "1", ClampMax = "30", EditCondition = "PatternType == ELaserPattern::Custom", EditConditionHides))
	int32 SecondaryCount = 1;

	// Spacing between lasers (in cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (ClampMin = "10.0"))
	float Spacing = 100.0f;

	// Spawned Lasers Orientation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	ELaserOrientation Orientation = ELaserOrientation::Horizontal;

	//Base height offset (Z-axis) for pattern
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	float HeighOffset = 0.0f;

	// Is randomize positions within spacing bounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	bool bRandomizePositions = false;

	// Randomize offset range  for bRandomizePositions is true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (EditCondition = "bRandomizePositions", EditConditionHides))
	float RandomOffsetRange = 20.0f;

	// Custom Spawn positions (local spawner, used with custom pattern)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern", meta = (EditCondition = "PatternType == ELaserPattern::Custom", EditConditionHides))
	TArray<FVector> CustomPositions;
};

UCLASS()
class LASERHALLWAY_API ALaserSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALaserObstacle> LaserObstacleClass;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0.01"))
	float SpawnInterval = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	bool bStartImmediate = true;

	// Array of patterns to randomly choose from spawn
	UPROPERTY(EditAnywhere, Category = "Patterns", meta = (TitleProperty = "PatternType"))
	TArray<FLaserPatternConfig> AvailablePatterns;

	// If true, chooses random pattern from AvailablePatterns. If false, cycle through.
	UPROPERTY(EditAnywhere, Category = "Patterns")
	bool bRandomizePatternSelection = true;

private:
	FTimerHandle SpawnTimerHandle;
	int32 CurrentPatternIndex = 0;

	UFUNCTION()
	void SpawnLaser();

	void SpawnPattern(const FLaserPatternConfig& Pattern);
	void SpawnSingleLaser(const FVector& Location, const FRotator& Rotation);
	TArray<FVector> GeneratePatternPositions(const FLaserPatternConfig& Pattern);

};
