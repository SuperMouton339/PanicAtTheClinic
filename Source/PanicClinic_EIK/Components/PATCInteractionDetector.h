// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "PATCInteractionDetector.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionListModified, AActor*, ClosestInteractable);

//Detects interactables in range of the owning pawn
//No RPCS on purpose: Collision exists on every machine, so the servers gets its own overlap events for every player.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PANICCLINIC_EIK_API UPATCInteractionDetector : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPATCInteractionDetector();
	UPROPERTY(BlueprintAssignable)
	FOnInteractionListModified OnInteractionListModified;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	
public:
		
	UFUNCTION()
	void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	
	
	// Read-only access to the list: returns a reference (no copy), and does not modify the component
	UFUNCTION(BlueprintCallable)
	const TArray<AActor*>& GetInteractionList() const
	{
		return InteractionList;
	}

	
	
	UFUNCTION(BlueprintPure)
	AActor* GetClosestInteractableItem() const;
	
protected:
	// Interactables currently overlapping this detector.
	// NOT replicated: each machine that needs it fills its own copy from its own overlaps.
	//  - Server (HasAuthority): validates ServerInteract requests
	//  - Owning client (IsLocallyControlled): picks the target and drives the prompt UI
	// Other players' characters on this machine (simulated proxies) keep it empty.
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> InteractionList;

	
};
