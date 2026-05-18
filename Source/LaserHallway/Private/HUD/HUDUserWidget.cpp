// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateHealthBar(1.0f);
	UpdateChargeShotBar(1.0f);
}

void UHUDUserWidget::UpdateHealthBar(float HealthPercent)
{
	if (!HealthBar)
	{
		return;
	}
	HealthBar->SetPercent(HealthPercent);
}

void UHUDUserWidget::UpdateChargeShotBar(float ChargePercent)
{
	if (!ChargeShotBar)
	{
		return;
	}
	ChargeShotBar->SetPercent(ChargePercent);
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
