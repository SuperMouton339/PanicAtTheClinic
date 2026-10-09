// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCPatientTreatmentComponent.h"

#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Character/PATCCharacter.h"
#include "PanicClinic_EIK/DataAssets/PATCTreatment.h"
#include "PanicClinic_EIK/Entities/PATCPatient.h"
#include "PanicClinic_EIK/Interfaces/PATCEquippableItem.h"
#include "PanicClinic_EIK/Minigames/PATCMinigame.h"


// Sets default values for this component's properties
UPATCPatientTreatmentComponent::UPATCPatientTreatmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPATCPatientTreatmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwningCharacter = Cast<APATCCharacter>(GetOwner());
}


void UPATCPatientTreatmentComponent::RegisterPatient(APATCPatient* NewPatient)
{
	if (!OwningCharacter->HasAuthority())
	{
		return;
	}
	PatientTreated = NewPatient;
	Treatment = NewPatient->GetTreatment();
	StartTreatmentDelegate = PatientTreated->TreatmentEvent.AddUObject(this, &UPATCPatientTreatmentComponent::StartTreatment);
}

void UPATCPatientTreatmentComponent::UnregisterPatient()
{
	if (!OwningCharacter->HasAuthority()) return;
	
	Treatment = nullptr;
	
	if (PatientTreated == nullptr) return;
	PatientTreated->TreatmentEvent.Remove(StartTreatmentDelegate);
	PatientTreated = nullptr;
}

void UPATCPatientTreatmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPATCPatientTreatmentComponent, PatientTreated);
	DOREPLIFETIME(UPATCPatientTreatmentComponent, Treatment);
}

void UPATCPatientTreatmentComponent::ProcessImmediateTreatment()
{
	if (!OwningCharacter->HasAuthority()) return;
	
	//This is here to consume a usage of the object after the treatment is handled.
	if (OwningCharacter->GetEquippedItem() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to consume."))
		IPATCEquippableItem::Execute_OnUseStarted(OwningCharacter->GetEquippedItem(), OwningCharacter);
		
	}
}


void UPATCPatientTreatmentComponent::StartTreatment_Implementation(APATCPatient* FocusedPatient)
{
	for (int i = 0; i < FocusedPatient->GetCurrentInteractors().Num(); i++)
	{
		if (FocusedPatient->GetCurrentInteractors()[i] == OwningCharacter)
		{
			if (FocusedPatient->GetTreatment()->MiniGamesBindings[i].MiniGameForPlayer == nullptr)
			{
				//Guard against nullptr.
				FocusedPatient->ReportTreatmentSuccess();
				break;
			}
			
			SpawnMinigame(FocusedPatient->GetTreatment()->MiniGamesBindings[i].MiniGameForPlayer, OwningCharacter);
			break;
		}
	}

	TriggerTreatmentAnim();
}

void UPATCPatientTreatmentComponent::TriggerTreatmentAnim_Implementation()
{
	StartTreatmentAnim();
}

