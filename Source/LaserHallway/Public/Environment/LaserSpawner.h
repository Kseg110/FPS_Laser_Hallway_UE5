// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserSpawner.generated.h"

class UBoxComponent;
class ALaserObstacle;

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

private:
	FTimerHandle SpawnTimerHandle;

	UFUNCTION()
	void SpawnLaser();

};
