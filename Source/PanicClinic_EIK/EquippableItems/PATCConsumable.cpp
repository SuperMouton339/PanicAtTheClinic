// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCConsumable.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Character/PATCCharacter.h"


// Sets default values
APATCConsumable::APATCConsumable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void APATCConsumable::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentUses = MaxUses;
}

void APATCConsumable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APATCConsumable, CurrentUses);
}

void APATCConsumable::OnRep_Uses()
{
	if (OwningPlayer == nullptr) return;
	
	if (OwningPlayer->IsLocallyControlled())
	{
		// Reasoning here is that only the owner (and if said owner is local) should see a change on his HUD related to the item's use.
		
		//TODO - Update UI
	}
}

void APATCConsumable::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	//TODO - Equip item (through server)
	
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Currently being equipped by someone."));
		
		if (APATCCharacter* Character = Cast<APATCCharacter>(InstigatorCharacter))
		{
			Character->ServerEquipNewItem(this);
			OwningPlayer = InstigatorCharacter;
			SetActorHiddenInGame(true);
		}
	}
		

	/*
	*Note: since this behavior should not differ between different types of equippable items, if we really want to avoid
	*every type sharing a base class, this could be handled in a component to avoid having to repeat functions - Jacob
	*/
	return;
}

void APATCConsumable::OnStarted_Implementation(ACharacter* InstigatorCharacter)
{
	//Since this is not blueprint callable, will need to be called by player controller
	CurrentUses -= 1;
		
	if (CurrentUses <= 0)
	{
		if (HasAuthority())
		{
			//TODO - Consume object
		}
	}
	return;
}
