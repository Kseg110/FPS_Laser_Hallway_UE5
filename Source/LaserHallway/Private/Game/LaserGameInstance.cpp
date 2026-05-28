// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LaserGameInstance.h"
#include "Game/HighScoreSaveGame.h"
#include "Kismet/GameplayStatics.h"

void ULaserGameInstance::Init()
{
	Super::Init();
	Load();
}

void ULaserGameInstance::Shutdown()
{
	Save();
	Super::Shutdown();
}

void ULaserGameInstance::Load()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
		if (UHighScoreSaveGame* SG = Cast<UHighScoreSaveGame>(Loaded))
		{
			HighScore = SG->HighScore;
		}
	}
}

void ULaserGameInstance::Save()
{
	UHighScoreSaveGame* SG = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
	if (SG)
	{
		SG->HighScore = HighScore;
		UGameplayStatics::SaveGameToSlot(SG, SlotName, UserIndex);
	}
}

void ULaserGameInstance::SetHighScore(int32 NewHighScore)
{
	if (NewHighScore > HighScore)
	{
		HighScore = NewHighScore;
		Save();
	}
}

void ULaserGameInstance::ResetHighScore()
{
	HighScore = 0;
	Save();
}



