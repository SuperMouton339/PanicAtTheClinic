// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCPatient.h"

#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Character/PATCCharacter.h"
#include "PanicClinic_EIK/Components/PATCPatientTreatmentComponent.h"
#include "PanicClinic_EIK/DataAssets/PATCTreatment.h"
#include "PanicClinic_EIK/EquippableItems/PATCBaseEquippable.h"


// Sets default values
APATCPatient::APATCPatient()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APATCPatient::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentInteractors.SetNum(2);
}

void APATCPatient::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	if (!HasAuthority()) return;

	if (bTreated) return;
	
	if (Treatments.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: Forgot to assign a treatment to patient !!!!!"))
		return;
	}
	
	//This method is dirty as hell and past the technical proof I'd like to avoid casting here as much as possible.
	APATCCharacter* ClinicCharacter = Cast<APATCCharacter>(InstigatorCharacter);
	if (ClinicCharacter == nullptr) return;

	if (CurrentInteractors.Contains(ClinicCharacter)) return;
	
	//This should become a function.
	for (auto Treatment : Treatments)
	{
		switch (Treatment->TreatmentType)
		{
		case EPATCTreatmentTypes::Simple: 
			{
				//Catch this here to avoid trying to do a IsA on a nullptr later.
				if (ClinicCharacter->GetEquippedItem() == nullptr && Treatment->RequiredEquippable != nullptr)
				{
					break;
				}
			
				if ((ClinicCharacter->GetEquippedItem() == nullptr && Treatment->RequiredEquippable == nullptr) || 
					ClinicCharacter->GetEquippedItem()->IsA(Treatment->RequiredEquippable) )
				{
					UE_LOG(LogTemp, Warning, TEXT("My problems have been fixed !!!"));
					if (UPATCPatientTreatmentComponent* TreatmentComp = ClinicCharacter->FindComponentByClass<UPATCPatientTreatmentComponent>())
					{
						TreatmentComp->ProcessImmediateTreatment();
					}
					Treatments.Remove(Treatment);
					MulticastTriggerRefreshUI();
					ProcessTreatmentEnd();
					return;
				}
				break;
			}
		case EPATCTreatmentTypes::Complex:
			{
				//This method of choosing what complex treatment is being processed is purely for the tech demo, later on we will have to write a real system that kind of behaves like an autocomplete.
				if (CurrentComplexTreatment == nullptr)
				{
					CurrentComplexTreatment = Treatment;
				}

				if (!FindInteractionSlot(ClinicCharacter)) break;
			
				bool bNaturalEnd = true;
			
				for (int i = 0; i < CurrentInteractors.Num(); ++i)
				{
					if (CurrentInteractors[i] == nullptr)
					{
						bNaturalEnd = false;
						break;
					}
				}
			
				if (bNaturalEnd)
				{
					TreatmentEvent.Broadcast(this);
					bTreatmentHappening = true;
					return;
				}
			}
		}
	}
}

void APATCPatient::ReportTreatmentSuccess()
{
	if (!HasAuthority()) return;
	
	SuccessCounter++;
	
	if (SuccessCounter >= CurrentInteractors.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("My problems have been fixed !!! (%i interactors, %i successes)"), CurrentInteractors.Num(), SuccessCounter);
		for (auto it : CurrentInteractors)
		{
			if (it == nullptr) continue;
			
			if (UPATCPatientTreatmentComponent* TreatmentComp = it->FindComponentByClass<UPATCPatientTreatmentComponent>())
			{
				TreatmentComp->ProcessImmediateTreatment();
			}
			it->MulticastTriggerTreatmentFX(false);
			TryUnregisterPlayer(it);
		}
		NotifyTreatmentEnded.Broadcast();
		
		if (CurrentComplexTreatment != nullptr)
		{
			Treatments.Remove(CurrentComplexTreatment);
			CurrentComplexTreatment = nullptr;
		}
		MulticastTriggerRefreshUI();
		
		ProcessTreatmentEnd();
	}
}

void APATCPatient::ReportTreatmentFailure()
{
	if (!HasAuthority()) return;
	UE_LOG(LogTemp, Warning, TEXT("Failed treatment"))
	SuccessCounter = 0;
	for (auto i : CurrentInteractors)
	{
		TryUnregisterPlayer(i);
	}
	NotifyTreatmentEnded.Broadcast();
}

void APATCPatient::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APATCPatient, bTreated);
}

void APATCPatient::MulticastTriggerCureFX_Implementation()
{
	StartCureFX();
}

void APATCPatient::TryUnregisterPlayer(APATCCharacter* Character)
{
	if (Character == nullptr) return;
	if (!CurrentInteractors.Contains(Character)) return;
	
	int i = CurrentInteractors.Find(Character);
	
	CurrentInteractors[i]->MulticastTriggerTreatmentFX(false);
	if (UPATCPatientTreatmentComponent* TreatmentComp = CurrentInteractors[i]->FindComponentByClass<UPATCPatientTreatmentComponent>())
	{
		TreatmentComp->UnregisterPatient();
	}
	CurrentInteractors[i] = nullptr;
	
}

bool APATCPatient::FindInteractionSlot(APATCCharacter* Character)
{
	if (!HasAuthority()) return false;

	if (CurrentComplexTreatment == nullptr) return false;
	
	for (int i = 0; i < CurrentComplexTreatment->MiniGamesBindings.Num(); ++i)
	{
		if (CurrentInteractors[i] != nullptr) continue;
		
		if (Character->GetEquippedItem() == nullptr && CurrentComplexTreatment->MiniGamesBindings[i].RequiredEquippable != nullptr ||
		Character->GetEquippedItem() != nullptr && CurrentComplexTreatment->MiniGamesBindings[i].RequiredEquippable == nullptr)
		{
			continue;
		}
		
		if ((Character->GetEquippedItem() == nullptr && CurrentComplexTreatment->MiniGamesBindings[i].RequiredEquippable == nullptr) || 
			Character->GetEquippedItem()->IsA(CurrentComplexTreatment->MiniGamesBindings[i].RequiredEquippable))
		{
			CurrentInteractors[i] = Character;
			if (UPATCPatientTreatmentComponent* TreatmentComp = CurrentInteractors[i]->FindComponentByClass<UPATCPatientTreatmentComponent>())
			{
				TreatmentComp->RegisterPatient(this);
			}
			Character->MulticastTriggerTreatmentFX(true);
			return true;
		}
	}
	return false;
}

void APATCPatient::ProcessTreatmentEnd()
{
	if (!HasAuthority()) return;
	if (!Treatments.IsEmpty()) return;
	MulticastTriggerCureFX();
	bTreated = true;
}

void APATCPatient::MulticastTriggerRefreshUI_Implementation()
{
	StartRefreshUI();
}

// Called every frame
void APATCPatient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
