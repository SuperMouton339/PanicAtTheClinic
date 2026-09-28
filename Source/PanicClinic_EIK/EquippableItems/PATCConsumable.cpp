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
	APawn* OwnerCharacter = Cast<APawn>(GetOwner());
	
	if (OwnerCharacter == nullptr) return;
	
	if (OwnerCharacter->IsLocallyControlled())
	{
		// Reasoning here is that only the owner (and if said owner is local) should see a change on his HUD related to the item's use.
		
		//TODO - Update UI
	}
}

void APATCConsumable::OnRep_Owner()
{
	Super::OnRep_Owner();
	
	// Collision is not replicated: each machine applies it locally from the replicated Owner.
	// Disabling collision fires EndOverlap on every detector touching the item, on this machine.
	if (GetOwner())
	{
		SetActorEnableCollision(false);
	}
	else
	{
		SetActorEnableCollision(true);
	}
}

void APATCConsumable::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	
	// Prevent a second player pressing E on the same frame and is refused because this Consumable has already an Owner
	if (GetOwner()) return;
	
	// Called by APATCCharacter::ServerInteract, so this always runs on the server.
	// The guard documents that assumption and protects against a future client-side call.
	if (HasAuthority())
	{
		
		if (APATCCharacter* Character = Cast<APATCCharacter>(InstigatorCharacter))
		{
			// The character decides if it can take the item (e.g. hands already full)
			bool bItemEquipped = Character->EquipNewItem(this);
			
			// Equip refused: leave the item untouched in the world
			if (!bItemEquipped) return;
			
			UE_LOG(LogTemp, Warning, TEXT("Currently being equipped by someone."));
			
			// Owner replicates to clients, which then run OnRep_Owner automatically
			SetOwner(InstigatorCharacter);
			
			// OnRep is never triggered on the server: so recalling mannually OnRep_Owner for the server
			OnRep_Owner();
			// bHidden is replicated: hides the item on every machine
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
	// BlueprintNativeEvent, NOT an RPC: runs on whichever machine calls it.
	// CurrentUses is replicated, so only the server may change it.
	// Intended caller: APATCCharacter::Server_StartUse (next session).
	if (!HasAuthority()) return;
	
	//Since this is not blueprint callable, will need to be called by Character
	CurrentUses -= 1;
	
	OnRep_Uses();
	if (CurrentUses <= 0)
	{
			//TODO - Consume object
		
	}
	return;
}
