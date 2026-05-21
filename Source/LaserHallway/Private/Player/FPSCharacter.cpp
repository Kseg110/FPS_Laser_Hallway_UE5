// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HUD/LaserHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FPSCharacter.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!FPSCameraComponent)
	{
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera")); // adds camera component to player blueprint
		FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
		FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
		FPSCameraComponent->bUsePawnControlRotation = true;
	}

	if (!FPSMeshComponent)
	{
		FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
		FPSMeshComponent->SetupAttachment(FPSCameraComponent);
		FPSMeshComponent->bCastDynamicShadow = false;
		FPSMeshComponent->CastShadow = false;
	}

	GetMesh()->SetOwnerNoSee(true);

	UE_LOG(LogTemp, Warning, TEXT("FPSCharacter Constructor call"));

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Charge = FMath::Clamp(Charge, 0.0f, 1.0f);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (PlayerMappingContext)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (Subsystem)
			{
				Subsystem->AddMappingContext(PlayerMappingContext, 0);
			}
		}
		if (ALaserHUD* LaserHUD = Cast<ALaserHUD>(PlayerController->GetHUD()))
		{
			LaserHUD->UpdateHealthBar(Health / MaxHealth);
			LaserHUD->UpdateChargeShotBar(Charge);
		}
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPSCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPSCharacter::EndJump);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AFPSCharacter::Fire);
		EnhancedInputComponent->BindAction(AltFireAction, ETriggerEvent::Started, this, &AFPSCharacter::AltFire);
	}
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		OnDmgPlayer(ActualDamage);
	}
	return ActualDamage;
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::EndJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	if (!ProjectileClass) return;

	// init projectiles relevant locaiton info
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	MuzzleOffset.Set(100.0f, 0.0f, -10.0f);

	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	FRotator MuzzleRotation = CameraRotation;
	MuzzleRotation.Pitch += 0.0f;

	// spawn projectile
	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Instantiate
	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (!Projectile) return;

	FVector LaunchDirection = MuzzleRotation.Vector();
	Projectile->FireInDirection(LaunchDirection);

	//OnDmgPlayer(10.0f); // for testing only
}

void AFPSCharacter::AltFire()
{
	if (Charge <= 0.0f) return;

	if (!ProjectileClass) return;

	// init projectiles relevant locaiton info
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	MuzzleOffset.Set(100.0f, 0.0f, -10.0f);

	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	FRotator MuzzleRotation = CameraRotation;
	MuzzleRotation.Pitch += 0.0f;

	// spawn projectile
	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Instantiate
	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (!Projectile) return;

	FVector LaunchDirection = MuzzleRotation.Vector();
	Projectile->FireInDirection(LaunchDirection);

	//Consume charge bar on charge shot
	Charge = 0.0f;

	//Update HUD
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ALaserHUD* LaserHUD = Cast<ALaserHUD>(PC->GetHUD()))
		{
			LaserHUD->UpdateChargeShotBar(Charge);
		}
	}
}

void AFPSCharacter::OnDmgPlayer(float DamageAmount)
{
	if (DamageAmount <= 0.0f) return;
	if (Health <= 0.0f) return;

	Health -= DamageAmount;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ALaserHUD* LaserHUD = Cast<ALaserHUD>(PC->GetHUD()))
		{
			LaserHUD->UpdateHealthBar(Health / MaxHealth);
		}
	}
}

float AFPSCharacter::GetChargePercent() const
{
	return FMath::Clamp(Charge, 0.0f, 1.0f);
}

void AFPSCharacter::RefillCharge()
{
	Charge = 1.0f;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ALaserHUD* LaserHUD = Cast<ALaserHUD>(PC->GetHUD()))
		{
			LaserHUD->UpdateChargeShotBar(Charge);
		}
	}
}