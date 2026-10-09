// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PATCBaseEntity.h"
#include "PanicClinic_EIK/DataAssets/PATCTreatment.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PATCPatient.generated.h"

enum class EPATCTreatmentTypes : uint8;
class APATCCharacter;
DECLARE_MULTICAST_DELEGATE_OneParam(TreatmentStarting, class APATCPatient*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTreatmentEnded);

UCLASS(Blueprintable, BlueprintType)
class PANICCLINIC_EIK_API APATCPatient : public APATCBaseEntity, public IPATCInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APATCPatient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn = true), Replicated)
	TArray<TObjectPtr<class UPATCTreatment>> Treatments;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerCureFX();
	void MulticastTriggerCureFX_Implementation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartCureFX();
	
	UFUNCTION(BlueprintCallable)
	void TryUnregisterPlayer(APATCCharacter* Character);
	
	bool FindInteractionSlot(APATCCharacter* Character);
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bTreated = false;
	UPROPERTY()
	TObjectPtr<UPATCTreatment> CurrentComplexTreatment;
		
	UPROPERTY(BlueprintReadOnly)
	TArray<class APATCCharacter*> CurrentInteractors;
	
	void ProcessTreatmentEnd();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerRefreshUI();
	void MulticastTriggerRefreshUI_Implementation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartRefreshUI();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteracted_Implementation(ACharacter* InstigatorCharacter) override;
	
	UFUNCTION(BlueprintPure)
	UPATCTreatment* GetTreatment()
	{
		return CurrentComplexTreatment;
	}

	UFUNCTION(BlueprintPure)
	TArray<class APATCCharacter*> GetCurrentInteractors()
	{
		return CurrentInteractors;
	}
	
	TreatmentStarting TreatmentEvent;
	
	int SuccessCounter = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bTreatmentHappening = false;
	
	UFUNCTION(BlueprintCallable)
	void ReportTreatmentSuccess();
	
	UFUNCTION(BlueprintCallable)
	void ReportTreatmentFailure();
	
	UPROPERTY(BlueprintAssignable)
	FTreatmentEnded NotifyTreatmentEnded;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
