// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERHALLWAY_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ChargeShotBar;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UFUNCTION()
	void UpdateHealthBar(float HealthPercent);

	UFUNCTION()
	void UpdateChargeShotBar(float ChargePercent);

	UFUNCTION()
	void UpdateScoringText(int Score);

	UFUNCTION()
	void SetCrosshairVisible(bool bVisible);

	UFUNCTION()
	void SetCrosshairTexture(UTexture2D* Texture, bool bMatchSize = true);
};
