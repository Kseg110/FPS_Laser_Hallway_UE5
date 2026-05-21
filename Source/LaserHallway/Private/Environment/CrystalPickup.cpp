// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/CrystalPickup.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "HUD/LaserHUD.h"
#include "Player/FPSCharacter.h"

// Sets default values
ACrystalPickup::ACrystalPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CrystalCollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void ACrystalPickup::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACrystalPickup::OnOverlap);
	}
	
}

// Called every frame
void ACrystalPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrystalPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherCompo, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor != PlayerPawn) return;

	if (AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor))
	{
		PlayerCharacter->RefillCharge();

		Destroy();
	}
}
