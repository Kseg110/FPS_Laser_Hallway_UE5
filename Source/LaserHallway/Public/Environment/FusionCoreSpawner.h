// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/FusionCore.h"
#include "GameFramework/Actor.h"
#include "FusionCoreSpawner.generated.h"

class UBoxComponent;
class AFusionCore;

UCLASS()
class LASERHALLWAY_API AFusionCoreSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFusionCoreSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFusionCore> FusionCoreClass;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0.01"))
	float SpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	bool bStartImmediate = true;

private:
	FTimerHandle SpawnTimerHandle;

	UFUNCTION()
	void SpawnFusionCore();
};
