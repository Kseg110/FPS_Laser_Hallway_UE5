// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuGameMode.h"
#include "UI/MenuPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMenuGameMode::AMenuGameMode()
{
	DefaultPawnClass = nullptr;

	HUDClass = nullptr;

	PlayerControllerClass = AMenuPlayerController::StaticClass();
}

