// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LASERHALLWAY_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HighScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundScoreText;

	UPROPERTY(meta = (BindWidget))
	UButton* ClearScoreButton;
	
	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void UpdateHighScoreText(int HighScore);

	UFUNCTION()
	void UpdateRoundScoreText(int RoundScore);

	UFUNCTION()
	void OnClearScoreButtonClicked();
};
