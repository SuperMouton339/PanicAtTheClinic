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
	
	UE_LOG(LogTemp, Warning, TEXT("%s CurrentUses: %d, HasAuthority: %s"),
	*GetName(), CurrentUses, HasAuthority() ? TEXT("True") : TEXT("False"));
	
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
	

}

void APATCConsumable::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	Super::OnInteracted_Implementation(InstigatorCharacter);
}

void APATCConsumable::OnUseStarted_Implementation(ACharacter* InstigatorCharacter)
{
	// BlueprintNativeEvent, NOT an RPC: runs on whichever machine calls it.
	// CurrentUses is replicated, so only the server may change it.
	// Intended caller: APATCCharacter::Server_StartUse (next session).
	
	// Instant treatment for the tech demo (no channeling).
	// TODO - If channeling comes back: start a server timer here, cancel it in OnUseStopped,
	// and move CurrentUses -= 1 to the timer's completion (a canceled heal must not burn a use).
	// TODO - Patient gate: refuse if no patient in range needs this TreatmentType (item-specific validation).
	
	if (!HasAuthority()) return;
	
	//Since this is not blueprint callable, will need to be called by Character
	CurrentUses -= 1;
	UE_LOG(LogTemp, Warning, TEXT("%s CurrentUses: %d, HasAuthority: %s"),
	*GetName(), CurrentUses, HasAuthority() ? TEXT("True") : TEXT("False"));
	OnRep_Uses();
	if (CurrentUses <= 0)
	{
		if (APATCCharacter* Character = Cast<APATCCharacter>(InstigatorCharacter))
		{
			Character->UnequipItem();
			
		}
		Destroy();
	}
	return;
}
