// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Interfaces/PATCDeath.h"


// Sets default values for this component's properties
UPATCHealthComponent::UPATCHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UPATCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;

		
//Comment this, thats for testing purposes.
// Note to self: do not test things in begin play EVER
		//TakeDamage(MaxHealth);
		
	}
}


// Called every frame
void UPATCHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UPATCHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPATCHealthComponent, CurrentHealth)
}

void UPATCHealthComponent::OnRep_Health()
{
	//TODO - Update UI
	UE_LOG(LogTemp, Warning, TEXT("Health replicated."))
}

void UPATCHealthComponent::GainHealth_Implementation(int HealAmount)
{
	if (!GetOwner()->HasAuthority()) return;

	int NewHealth = CurrentHealth + HealAmount;
	
	if (NewHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	else
	{
		CurrentHealth = NewHealth;
	}
}

void UPATCHealthComponent::TakeDamage_Implementation(int DamageAmount)
{
	if (!GetOwner()->HasAuthority()) return;

	int NewHealth = CurrentHealth - DamageAmount;


	UE_LOG(LogTemp, Warning, TEXT("Took damage"))
	if (NewHealth <= 0)
	{
		CurrentHealth = 0;
		if (GetOwner()->Implements<UPATCDeath>())
		{
			IPATCDeath::Execute_OnDeath(GetOwner());
		}
	}

	else
	{
		CurrentHealth = NewHealth;
	}
}

