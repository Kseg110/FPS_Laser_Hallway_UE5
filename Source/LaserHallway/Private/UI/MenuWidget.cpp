// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Game/LaserGameInstance.h"
#include "UI/MenuWidget.h"

void UMenuWidget::NativeConstruct()
{
	StartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartGameClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitClicked);
	ClearScoreButton->OnClicked.AddDynamic(this, &UMenuWidget::OnClearScoreButtonClicked);

	if (GetWorld())
	{
		if (ULaserGameInstance* GI = Cast<ULaserGameInstance>(GetWorld()->GetGameInstance()))
		{
			UpdateHighScoreText(GI->GetHighScore());
		}
	}
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
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameMap"));
	}
}

void UMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMenuWidget::UpdateHighScoreText(int HighScore)
{
	if (!HighScoreText) return;
	FString S = FString::Printf(TEXT("High Score: %d"), HighScore);
	HighScoreText->SetText(FText(FText::FromString(S)));
}

void UMenuWidget::UpdateRoundScoreText(int RoundScore)
{
	if (!RoundScoreText) return;
	FString S = FString::Printf(TEXT("Score: %d"), RoundScore);
	RoundScoreText->SetText(FText::FromString(S));
}

void UMenuWidget::OnClearScoreButtonClicked()
{
	if (!GetWorld()) return;
	if (ULaserGameInstance* GI = Cast<ULaserGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->ResetHighScore();
		UpdateHighScoreText(0);
	}
}

