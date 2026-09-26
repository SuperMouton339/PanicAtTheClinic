// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCInteractionDetector.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"


// Sets default values for this component's properties
UPATCInteractionDetector::UPATCInteractionDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	
	
	PrimaryComponentTick.bCanEverTick = true;
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

void UPATCInteractionDetector::ServerAddToInteractionList_Implementation(AActor* ActorToAdd)
{
	//TODO - Add verification that the interaction is in fact valid on the server side.
	
	InteractionList.Add(ActorToAdd);
}

void UPATCInteractionDetector::ServerRemoveFromInteractionList_Implementation(AActor* ActorToRemove)
{
	//TODO - Add verification that the interaction is in fact valid on the server side.
	InteractionList.Remove(ActorToRemove);
}


// Called every frame
void UPATCInteractionDetector::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPATCInteractionDetector::OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{		

	//I don't do a check for local here because server requests should only be validated when the client owns the PC
	if (!GetOwner()->HasAuthority() && GetOwner()->HasLocalNetOwner() && OtherActor->Implements<UPATCInteractable>())
	{

		ServerAddToInteractionList(OtherActor);
	}
}

void UPATCInteractionDetector::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//I don't do a check for local here because server requests should only be validated when the client owns the PC
	if (!GetOwner()->HasAuthority() && GetOwner()->HasLocalNetOwner() && OtherActor->Implements<UPATCInteractable>())
	{

		ServerRemoveFromInteractionList(OtherActor);
	}
}

void UPATCInteractionDetector::OnRep_InteractionList()
{
	//TODO - Update UI to show interactable options.
	for (auto it : InteractionList)
	{
		
	}
}

void UPATCInteractionDetector::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPATCInteractionDetector, InteractionList);
}

