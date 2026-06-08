// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"
//#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Health to max value (100)
	Health = MaxHealth;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::InitializeHealth(float InitMaxHealth)
{
	MaxHealth = InitMaxHealth;
	Health = MaxHealth;
}

float UHealthComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || Health <= 0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = FMath::Min(DamageAmount, Health);
	Health -= ActualDamage;

	// Health changes
	OnHealthChanged.Broadcast(Health, ActualDamage);

	// dead check
	if (Health <= 0.0f)
	{
		OnDeath.Broadcast();
	}
	return ActualDamage;
}

float UHealthComponent::GetHealthPercent() const
{
	return MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;
}