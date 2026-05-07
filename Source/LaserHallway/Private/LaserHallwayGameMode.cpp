// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserHallwayGameMode.h"

void ALaserHallwayGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	// Game running debug message
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Game Mode running!"));

}
