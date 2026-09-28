// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCInteractionDetector.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"


// Sets default values for this component's properties
UPATCInteractionDetector::UPATCInteractionDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	
	
	PrimaryComponentTick.bCanEverTick = false;
	OnComponentBeginOverlap.AddDynamic(this, &UPATCInteractionDetector::OnComponentOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UPATCInteractionDetector::OnComponentOverlapEnd);
	// ...
}


// Called when the game starts
void UPATCInteractionDetector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



void UPATCInteractionDetector::OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{	
	
	// Only pawns can be locally controlled; ignore if attached to anything else
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		// Server needs the list to validate, owning client needs it for targeting/UI.
		// The listen-server host is both, but this single if still adds the actor once.
		// HasAuthority first: cheapest check, short-circuits on the server.
		if (( GetOwner()->HasAuthority() || OwnerPawn->IsLocallyControlled()) && OtherActor->Implements<UPATCInteractable>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner Name: %s, HasAuthority : %s, Is LocallyControlled : %s"), *GetOwner()->GetName(), 
				(GetOwner()->HasAuthority() ? TEXT("True") : TEXT("False")) ,(OwnerPawn->IsLocallyControlled() ? TEXT("True") : TEXT("False")));
			
			// AddUnique: an item with several collision components overlaps several times
			InteractionList.AddUnique(OtherActor);
		}
	}
	
}

void UPATCInteractionDetector::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (( GetOwner()->HasAuthority() || OwnerPawn->IsLocallyControlled()) && OtherActor->Implements<UPATCInteractable>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner Name: %s, HasAuthority : %s, Is LocallyControlled : %s"), *GetOwner()->GetName(), 
				(GetOwner()->HasAuthority() ? TEXT("True") : TEXT("False")) ,(OwnerPawn->IsLocallyControlled() ? TEXT("True") : TEXT("False")));
			
			// Same machines as OverlapBegin, so the list stays symmetrical
			InteractionList.Remove(OtherActor);
		}
	}
}



