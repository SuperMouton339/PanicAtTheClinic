// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCHealthComponent.h"

#include "Misc/DataValidation.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Interfaces/PATCHealthEvents.h"


// Sets default values for this component's properties
UPATCHealthComponent::UPATCHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
	
}


// Called when the game starts
void UPATCHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
	}
	OnPlayerHealthChanged.Broadcast(GetCurrentHealth());
	//Singular check for interface implementation.
	bImplementsHealthInterface = GetOwner()->Implements<UPATCHealthEvents>();
	// ...
}

void UPATCHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPATCHealthComponent, CurrentHealth)
}


void UPATCHealthComponent::OnRep_Health()
{
	OnPlayerHealthChanged.Broadcast(GetCurrentHealth());
	UE_LOG(LogTemp, Warning, TEXT("Health replicated."))
}

void UPATCHealthComponent::GainHealth(int HealAmount)
{
	//This function should only ever be running on server.
	if (!GetOwner()->HasAuthority()) return;
	if (CurrentHealth <= 0) return;
	
	
	CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
	OnRep_Health();
	if (bImplementsHealthInterface)
	{
		IPATCHealthEvents::Execute_OnHealthReceived(GetOwner());
	}
	
}

void UPATCHealthComponent::ReviveCharacter(int HealAmount)
{
	if (!GetOwner()->HasAuthority() || CurrentHealth > 0) return;
	
	CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
	OnRep_Health();
	if (bImplementsHealthInterface)
	{
		IPATCHealthEvents::Execute_OnHealthReceived(GetOwner());
	}
	
}

void UPATCHealthComponent::TakeDamage(int DamageAmount)
{
	//This function should only ever be running on server.
	if (!GetOwner()->HasAuthority()) return;
	
	if (CurrentHealth <= 0) return;
	
	CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0);
	OnRep_Health();
	if (bImplementsHealthInterface)
	{
		IPATCHealthEvents::Execute_OnDamageTaken(GetOwner());
	}
	
	if (CurrentHealth == 0)
	{
		
		if (bImplementsHealthInterface)
		{
			//Call the interface to let the actor process its death however it wants to.
			IPATCHealthEvents::Execute_OnDeath(GetOwner());
		}
	}
}

