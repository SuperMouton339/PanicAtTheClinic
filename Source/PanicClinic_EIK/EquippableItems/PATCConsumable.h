// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanicClinic_EIK/Enums/PATCTreatmentTypes.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PanicClinic_EIK/Interfaces/PATCEquippableItem.h"
#include "PATCConsumable.generated.h"

UCLASS()
class PANICCLINIC_EIK_API APATCConsumable : public AActor, public IPATCInteractable, public IPATCEquippableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCConsumable();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	int MaxUses = 1;
	
	//Should possibly be replicated ? Only way I see it not needing to be replicated is if once an item is dropped it cant be picked back up. - Jacob
	UPROPERTY(ReplicatedUsing = OnRep_Uses)
	int CurrentUses = 1;
	
	UPROPERTY(EditDefaultsOnly)
	EPATCTreatmentTypes TreatmentType;
	
	UPROPERTY()
	APlayerController* OwningPlayerController;
	
	UFUNCTION()
	void OnRep_Uses();
	
public:
	
	virtual void OnInteracted(APlayerController* InstigatorPC) override;

	virtual void OnStarted(APlayerController* InstigatorPC) override;

	virtual void OnTriggered(APlayerController* InstigatorPC) override
	{
		return;
	}
};