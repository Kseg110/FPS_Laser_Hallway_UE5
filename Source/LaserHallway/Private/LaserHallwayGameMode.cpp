// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserHallwayGameMode.h"

void ALaserHallwayGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	//debug message for 5 seconds
	//Test if game mode has started correctly
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("LaserHallway game mode started!"));
}
