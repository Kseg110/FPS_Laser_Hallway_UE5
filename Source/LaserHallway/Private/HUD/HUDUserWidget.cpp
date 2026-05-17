// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateHealthBar(1.0f);
	UpdateChargeShotText(3);
}

void UHUDUserWidget::UpdateHealthBar(float HealthPercent)
{
	if (!HealthBar)
	{
		return;
	}
	HealthBar->SetPercent(HealthPercent);
}

void UHUDUserWidget::UpdateChargeShotText(int ShotCount)
{
	if (!ChargeShotText)
	{
		return;
	}
	FString ChargeShotString = FString::Printf(TEXT("Charge Shots: %d"), ShotCount);
	ChargeShotText->SetText(FText::FromString(ChargeShotString));
}

void UHUDUserWidget::UpdateScoringText(int Score)
{
	if (!ScoreText)
	{
		return;
	}
	FString ScoreString = FString::Printf(TEXT("Score: %d"), Score);
	ScoreText->SetText(FText::FromString(ScoreString));
}

void UHUDUserWidget::SetCrosshairVisible(bool bVisible)
{
	if (!CrosshairImage) return;
	CrosshairImage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UHUDUserWidget::SetCrosshairTexture(UTexture2D* Texture, bool bMatchSize)
{
	if (!CrosshairImage) return;
	if (Texture)
	{
		CrosshairImage->SetBrushFromTexture(Texture, bMatchSize);
		CrosshairImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CrosshairImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
