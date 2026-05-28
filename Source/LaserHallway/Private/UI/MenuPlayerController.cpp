// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MenuPlayerController.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("MenuCamera")), Found);
	if (Found.Num() > 0)
	{
		if (ACameraActor* Cam = Cast<ACameraActor>(Found[0]))
		{
			SetViewTarget(Cam);
			return;
		}
	}


}
