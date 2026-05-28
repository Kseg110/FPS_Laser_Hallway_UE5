// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UI/MenuWidget.h"

void UMenuWidget::NativeConstruct()
{
	StartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGameClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitClicked);
}

void UMenuWidget::OnStartGameClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		// removes menu UI and grabs player owning controller 
		RemoveFromParent();

		// returns input to game
		FInputModeGameOnly GameInput;
		PC->SetInputMode(GameInput);
		PC->bShowMouseCursor = false;
	}

	if (GetWorld())
	{
		// switch to gamescene on click
		//FName LevelName(TEXT("GameMap"));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameMap"));
	}
}

void UMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

