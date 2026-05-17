// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LaserHUD.generated.h"

class UHUDUserWidget;
class APlayerController;

/**
 * 
 */
UCLASS()
class LASERHALLWAY_API ALaserHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALaserHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UHUDUserWidget> HUDWidgetClass;

	UPROPERTY(Transient)
	UHUDUserWidget* HUDWidgetInstance;

	UFUNCTION()
	void UpdateHealthBar(float HealthPercent);

	UFUNCTION()
	void UpdateChargeShotText(int ShotCount);

	UPROPERTY()
	int32 Score;

	UFUNCTION()
	void AddScore(int32 Amount);
};
