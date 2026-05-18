// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrystalPickup.generated.h"

UCLASS()
class LASERHALLWAY_API ACrystalPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystalPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Environment")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Environment")
	UStaticMeshComponent* MeshComponent;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherCompo, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
