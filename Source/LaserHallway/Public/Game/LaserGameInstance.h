// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LaserGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LASERHALLWAY_API ULaserGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	int32 GetHighScore() const { return HighScore; }

	UFUNCTION(BlueprintCallable)
	int32 GetLastRoundScore() const { return LastRoundScore; }

	UFUNCTION(BlueprintCallable)
	void SetHighScore(int32 NewHighScore);

	UFUNCTION(BlueprintCallable)
	void ResetHighScore();

private:
	void Load();
	void Save();

	int32 HighScore = 0;
	int32 LastRoundScore = 0;
	FString SlotName = TEXT("HighScoreSlot");
	uint32 UserIndex = 0;
	
};
