// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LASERHALLWAY_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMenuGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
