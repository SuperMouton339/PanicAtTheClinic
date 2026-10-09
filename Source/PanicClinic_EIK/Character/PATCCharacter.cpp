// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Components/PATCInteractionDetector.h"
#include "PanicClinic_EIK/EquippableItems/PATCConsumable.h"
#include "PanicClinic_EIK/Interfaces/PATCEquippableItem.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PanicClinic_EIK/Interfaces/PATCWeapon.h"
#include "PanicClinic_EIK/Components/PATCHealthComponent.h"
#include "PanicClinic_EIK/GameModes/PATCPlayerState.h"

DEFINE_LOG_CATEGORY(LogPATCNetCharacter);

// Sets default values
APATCCharacter::APATCCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	bUseControllerRotationYaw = false;
	
	InteractionDetector = CreateDefaultSubobject<UPATCInteractionDetector>(TEXT("InteractableDetector"));
	InteractionDetector->SetCollisionProfileName(TEXT("InteractionDetector"));
	InteractionDetector->SetupAttachment(GetRootComponent());
	
	ReviveInteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("ReviveInteractionZone"));
	ReviveInteractionZone->SetupAttachment(GetRootComponent());
	ReviveInteractionZone->SetCollisionProfileName(TEXT("Interactable"));
	ReviveInteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->bOrientRotationToMovement= true;
	
	PlayerHealthComponent = CreateDefaultSubobject<UPATCHealthComponent>(TEXT("HealthComponent"));
	PlayerHealthComponent->SetIsReplicated(true);
	
}

void APATCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	RefreshPlayerNumber();
}

void APATCCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	RefreshPlayerNumber();
}


// Called when the game starts or when spawned
void APATCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RefreshPlayerNumber();
	
	ApplyMovementSettings();
	
	//Guard If the instance doesn't have a LocalPlayer, return (for the player UI)
	if (!IsLocallyControlled()) return;
	
}

void APATCCharacter::RefreshPlayerNumber()
{
	if (!HasActorBegunPlay()) return;
	
	APATCPlayerState* PatcPlayerState = Cast<APATCPlayerState>(GetPlayerState());
	
	if (!IsValid(PatcPlayerState))
	{
		UE_LOG(LogPATCNetCharacter, Log, TEXT("No PATC player state")); 
		return;
	}
	int32 PlayerNumber = PatcPlayerState->GetPlayerNumber();
	if ( PlayerNumber > 0)
	{
		OnRefreshPlayerNumber(PlayerNumber);
	}
	UE_LOG(LogPATCNetCharacter, Log, TEXT("HasAuthority? : %s, Character Name: %s, Player Number: %d"), 
		(HasAuthority() ? TEXT("True") : TEXT("False")), *GetName(), PatcPlayerState->GetPlayerNumber());
}

void APATCCharacter::Move(const FInputActionValue& Value)
{
	//Convert the InputAction value to a FVector2D
	const FVector2D Axis = Value.Get<FVector2D>();
	
	//Because it is an TopDown Game, the Camera doesn't turn. (Up is actually up)
	//FVector Right = X (-1 = Left)
	AddMovementInput(FVector::ForwardVector, Axis.Y);
	AddMovementInput(FVector::RightVector, Axis.X);
}

void APATCCharacter::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	if (!HasAuthority()) return;
	if (!IsValid(InstigatorCharacter) || InstigatorCharacter == this ) return;
	if (!bIsDowned) return;
	APATCCharacter* OtherPlayer = Cast<APATCCharacter>(InstigatorCharacter);
	if (!IsValid(OtherPlayer) || OtherPlayer->IsDowned()) return;
	
	PlayerHealthComponent->ReviveCharacter(HealAmountOnRevive);
	
	
}

void APATCCharacter::ServerInteract_Implementation(AActor* ActorToInteract)
{
	// Player is down therefore can't interact!
	if (bIsDowned)
	{
		UE_LOG(LogPATCNetCharacter,Warning,TEXT("You are down you can't do that")); 
		return;
	}
	// Server-side validation: the client only proposes a target, the server decides.
	// Reject null and anything not in the server's own overlap list (range check without trusting the client).
	if (!ActorToInteract || !InteractionDetector->GetInteractionList().Contains(ActorToInteract)) return;
	
	// The detector only lists IPATCInteractable actors, so Execute_ is safe once the list check passed
	IPATCInteractable::Execute_OnInteracted(ActorToInteract, this);
	
}

void APATCCharacter::ServerStartUse_Implementation()
{
	// Player is down therefore can't use Items
	if (bIsDowned)
	{
		UE_LOG(LogPATCNetCharacter,Warning,TEXT("You are down you can't do that")); 
		
		return;
	}
	
	// Network entry point: validate everything. Execute_ hits a check() (crash) if the interface is missing.
	if (!IsValid(EquippedItem)|| !EquippedItem->Implements<UPATCEquippableItem>())
	{
		// TODO - Remove UE_LOG
		UE_LOG(LogPATCNetCharacter,Warning,TEXT("Refused"));
		return;
	}
	
	// TODO - Remove UE_LOG
	UE_LOG(LogPATCNetCharacter,Warning,TEXT("Character Name: %s : Has Authority? %s"), *GetName(), 
		(HasAuthority() ? TEXT("True") : TEXT("False")));
	IPATCEquippableItem::Execute_OnUseStarted(EquippedItem, this);
	
}

void APATCCharacter::ServerStopUse_Implementation()
{
	if (!IsValid(EquippedItem) || !EquippedItem->Implements<UPATCEquippableItem>()) return;
	UE_LOG(LogTemp,Warning,TEXT("Character Name: %s : Has Authority? %s"), *GetName(), 
		(HasAuthority() ? TEXT("True") : TEXT("False")));
	IPATCEquippableItem::Execute_OnUseStopped(EquippedItem, this);
}

void APATCCharacter::OnRep_EquippedItem()
{
	OnEquippedChanged();
	
	if (IsLocallyControlled())
	{
		//TODO - Apply changes to UI
		// TODO - nullptr means the item was consumed/dropped: clear the UI here instead of returning.
		// Don't rely on OnRep_Uses reaching 0: a destroyed actor may close its channel before the last value is sent.
		if (EquippedItem == nullptr) return;
		
		// TODO - Move debug UE_LOGs to a custom log category (e.g. LogPATCNet) or remove them before the demo.
		UE_LOG(LogTemp, Warning, TEXT("%s equipped %s"), *GetNetOwner()->GetName(), *EquippedItem->GetName());
	}
	
	if (!IsLocallyControlled())
	{
		//TODO - Show every player that this player picked up an item (Player himself should've already handled the animations somewhere else)
		
		UE_LOG(LogTemp, Warning, TEXT("Owner Name: %s, HasAuthority : %s, Is LocallyControlled : %s"), *this->GetName(), 
				(HasAuthority() ? TEXT("True") : TEXT("False")) ,(IsLocallyControlled() ? TEXT("True") : TEXT("False")));
	}
}

void APATCCharacter::ServerAttack_Implementation()
{
	if (bIsDowned)
	{
		UE_LOG(LogPATCNetCharacter,Warning,TEXT("You are down you can't do that")); 
		return;
	}
	if (!HasAuthority()) return;

	if (!bHasWeapon) return;
	
	if (EquippedItem == nullptr) return;
	
	if (EquippedItem->Implements<UPATCWeapon>())
	{
		IPATCWeapon::Execute_OnAttack(EquippedItem, this);
	}
}

void APATCCharacter::ServerDropItem_Implementation()
{
	if (bIsDowned)
	{
		UE_LOG(LogPATCNetCharacter,Warning,TEXT("You are down you can't do that")); 
		
		return;
	}
	
	DropItemSequence();
	
}

void APATCCharacter::MulticastTriggerAttackAnim_Implementation()
{
	StartAttackAnim();
}


void APATCCharacter::OnRep_IsDowned()
{
	UE_LOG(LogPATCNetCharacter, Warning, TEXT("Owner Name: %s, Serveur?: %s, IsDown?: %s"), *this->GetName(),HasAuthority() ? 
		TEXT("True") : TEXT("False"), bIsDowned?TEXT("True") : TEXT("False"));
	if (bIsDowned)
	{
		CharacterMovementComp->DisableMovement();
		ReviveInteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		CharacterMovementComp->SetDefaultMovementMode();
		ReviveInteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	OnIsDownChanged.Broadcast(bIsDowned);
}

// Called every frame
void APATCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APATCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//if there's a reference MoveAction to begin with
	if (MoveAction)
	{
		//verify if there's a reference to an EIC in PlayerInputComponent
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			//Bind the MoveAction in Triggered Event for APATCCharacter Move inside the EIC
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APATCCharacter::Move);
		}
	}
	
}

bool APATCCharacter::EquipNewItem(AActor* ActorToEquip)
{
	// Server only: EquippedItem is replicated, clients must never write it
	if (!HasAuthority() || !ActorToEquip) return false;
	
	// Only equippable items, and only if the hands are empty
	if (ActorToEquip->Implements<UPATCEquippableItem>() && EquippedItem == nullptr)
	{
		// Replicates to every client, which then runs OnRep_EquippedItem automatically
		EquippedItem = ActorToEquip;
		
		// OnRep is never triggered on the server,
		// Nice to know: the listen-server host is also a player and must see every pickup (its own and others)
		OnRep_EquippedItem();

		if (EquippedItem->Implements<UPATCWeapon>())
		{
			bHasWeapon = true;
		}
		return true;
	}
	
	// Refused: nothing changed, the caller must not treat the item as picked up
	return false;
}
// Server-side unequip. NOT an RPC: only called by server code (e.g. a consumed item).
void APATCCharacter::UnequipItem()
{
	// Server only: EquippedItem is replicated, clients must never write it
	if (!HasAuthority() || !EquippedItem) return;
	
	// Replicates to every client, which then runs OnRep_EquippedItem automatically
	EquippedItem = nullptr;
	bHasWeapon = false;
	
	//the host must update too
	OnRep_EquippedItem();
	
}

void APATCCharacter::DropItemSequence()
{
	if (!HasAuthority() || !EquippedItem) return;
	
	if (!IsValid(EquippedItem) ||!EquippedItem->Implements<UPATCEquippableItem>()) return;
	
	IPATCEquippableItem::Execute_OnUseStopped(EquippedItem, this);
	
	IPATCEquippableItem::Execute_OnDropped(EquippedItem, GetActorLocation() + GetActorForwardVector() * DropItemLength);
	UnequipItem();
}

void APATCCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APATCCharacter, EquippedItem);
	DOREPLIFETIME(APATCCharacter, bHasWeapon);
	DOREPLIFETIME(APATCCharacter, bIsDowned);
}

void APATCCharacter::MulticastTriggerTreatmentFX_Implementation(bool bInState)
{
	ProcessTreatmentFX(bInState);
}

void APATCCharacter::SetIsDowned(bool bInState)
{	
	if (bIsDowned == bInState) return;
	if (!HasAuthority()) return;
	
	// When down, drop EquippedItems
	if (bInState)
	{
		DropItemSequence();
	}
	
	bIsDowned = bInState;
	OnRep_IsDowned();
}

bool APATCCharacter::IsDowned() const
{
	return bIsDowned;
}

UPATCHealthComponent* APATCCharacter::GetPlayerHealthComponent() const
{
	return PlayerHealthComponent;
}

void APATCCharacter::OnHealthReceived_Implementation()
{
	if (bIsDowned && PlayerHealthComponent->GetCurrentHealth() > 0)
	{
		SetIsDowned(false);
	}
	UE_LOG(LogPATCNetCharacter, Warning, TEXT("Owner Name: %s, Serveur?: %s, Health received"),*this->GetName(), HasAuthority() ? TEXT("True") : TEXT("False"));
}

void APATCCharacter::OnDeath_Implementation()
{
	UE_LOG(LogPATCNetCharacter, Warning, TEXT("Owner Name: %s, Serveur?: %s, Death"), *this->GetName(),HasAuthority() ? 
		TEXT("True") : TEXT("False"));
	
	
	SetIsDowned(true);
}

void APATCCharacter::OnDamageTaken_Implementation()
{
	UE_LOG(LogPATCNetCharacter, Warning, TEXT("Owner Name: %s, Serveur?: %s, Damage taken"), *this->GetName(),HasAuthority() ? TEXT("True") : TEXT("False"));
}

void APATCCharacter::ApplyMovementSettings()
{
	CharacterMovementComp->RotationRate = FRotator(0.0f,RotationRate,0.0f);
}

