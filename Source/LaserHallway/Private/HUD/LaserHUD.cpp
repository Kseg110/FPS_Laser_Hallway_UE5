// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LaserHUD.h"
#include "HUD/HUDUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

ALaserHUD::ALaserHUD()
{
	HUDWidgetInstance = nullptr;
}

void ALaserHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!HUDWidgetClass)
	{
		return;
	}

	if (APlayerController* PC = GetOwningPlayerController())
	{
		HUDWidgetInstance = CreateWidget<UHUDUserWidget>(PC, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			HUDWidgetInstance->UpdateHealthBar(1.0f);
			HUDWidgetInstance->UpdateChargeShotText(3);
		}
	}
}

void ALaserHUD::UpdateHealthBar(float HealthPercent)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->UpdateHealthBar(HealthPercent);
	}
}

void ALaserHUD::UpdateChargeShotText(int ShotCount)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->UpdateChargeShotText(ShotCount);
	}
}

