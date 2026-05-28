// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LASERHALLWAY_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

protected:
	UPROPERTY()
	UUserWidget* MenuWidget;
	
};
