// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Components/PATCInteractionDetector.h"
#include "PanicClinic_EIK/Interfaces/PATCEquippableItem.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"

// Sets default values
APATCCharacter::APATCCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	
	InteractionDetector = CreateDefaultSubobject<UPATCInteractionDetector>(TEXT("InteractableDetector"));
	InteractionDetector->SetCollisionProfileName(TEXT("InteractionDetector"));
	InteractionDetector->SetupAttachment(GetRootComponent());
}


// Called when the game starts or when spawned
void APATCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Guard If the instance doesn't have a LocalPlayer, return
	if (!IsLocallyControlled())
	{
		return;
	}
	
	
	
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

void APATCCharacter::ServerInteract_Implementation(AActor* ActorToInteract)
{
	if (ActorToInteract->Implements<UPATCInteractable>())
	{
		IPATCInteractable::Execute_OnInteracted(ActorToInteract, this);
	}
}

void APATCCharacter::OnRep_EquippedItem()
{


	if (IsLocallyControlled())
	{	//TODO - Apply changes to UI
		if (EquippedItem == nullptr) return;
		UE_LOG(LogTemp, Warning, TEXT("%s equipped %s"), *GetNetOwner()->GetName(), *EquippedItem->GetName());
	}
	
	if (!IsLocallyControlled())
	{
		//TODO - Show every player that this player picked up an item (Player himself should've already handled the animations somewhere else)
	}
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

void APATCCharacter::ServerEquipNewItem_Implementation(AActor* ActorToEquip)
{
	if (ActorToEquip->Implements<UPATCEquippableItem>())
	{
		EquippedItem = ActorToEquip;
	}
}

void APATCCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APATCCharacter, EquippedItem);
}

