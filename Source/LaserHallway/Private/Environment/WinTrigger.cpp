// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Environment/WinTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FPSCharacter.h"

// Sets default values
AWinTrigger::AWinTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WinTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinTriggerBox"));
	RootComponent = WinTriggerBox;

	WinTriggerBox->InitBoxExtent(FVector(200.0f, 200.0f, 100.0f));
	WinTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WinTriggerBox->SetCollisionObjectType(ECC_WorldStatic);
	WinTriggerBox->SetGenerateOverlapEvents(true);
	WinTriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WinTriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void AWinTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (WinTriggerBox)
	{
		WinTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWinTrigger::OnOverlap);
	}
	
}

// Called every frame
void AWinTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWinTrigger::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherCompo, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	// Only reacts to the player character 
	if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor))
	{
		// Load main menu level
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	}
}
