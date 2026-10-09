// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCBaseEquippable.h"

#include "PanicClinic_EIK/Character/PATCCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
APATCBaseEquippable::APATCBaseEquippable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicatingMovement(true);
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);
	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh -> SetupAttachment(RootSceneComponent);
	
}

// Called when the game starts or when spawned
void APATCBaseEquippable::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		ItemInitialRotation = GetActorRotation();
	}
}

// Called every frame
void APATCBaseEquippable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APATCBaseEquippable::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	IPATCInteractable::OnInteracted_Implementation(InstigatorCharacter);

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
			SetOwner(Character);
			
			// OnRep is never triggered on the server: so recalling mannually OnRep_Owner for the server
			OnRep_Owner();
			// bHidden is replicated: hides the item on every machine
			SetActorHiddenInGame(true);
		}
	}
}
//Function called from Server_DropItem in PATCCharacter server side
void APATCBaseEquippable::OnDropped_Implementation(FVector DroppedPosition)
{
	IPATCEquippableItem::OnDropped_Implementation(DroppedPosition);
	
	if (HasAuthority())
	{
		// Detach the equipped item attached to the Character on BP side
		
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		SetActorLocationAndRotation(DroppedPosition, ItemInitialRotation);
		SetOwner(nullptr);
        	
		OnRep_Owner();
        	
		SetActorHiddenInGame(false);
	}
	
	
}

void APATCBaseEquippable::OnRep_Owner()
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

