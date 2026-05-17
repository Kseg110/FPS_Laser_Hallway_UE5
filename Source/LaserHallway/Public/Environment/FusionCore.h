// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FusionCore.generated.h"

UCLASS()
class LASERHALLWAY_API AFusionCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFusionCore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Environment")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Environment")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BobbingAmplitude = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BobbingSpeed = 1.0f;

private:
	// starting location for bobbing
	FVector StartLocation;
	float RunningTime = 0.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
