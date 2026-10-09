// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PATCPatientTreatmentComponent.generated.h"


class APATCMinigame;
class UPATCMiniGameWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PANICCLINIC_EIK_API UPATCPatientTreatmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPATCPatientTreatmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<class APATCCharacter> OwningCharacter;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<class APATCPatient> PatientTreated;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<class UPATCTreatment> Treatment;

	UFUNCTION(Server, Reliable)
	void StartTreatment(APATCPatient* FocusedPatient);
	void StartTreatment_Implementation(APATCPatient* FocusedPatient);
	
	UFUNCTION(NetMulticast, Unreliable)
	void TriggerTreatmentAnim();
	void TriggerTreatmentAnim_Implementation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnMinigame(TSubclassOf<APATCMinigame> Minigame, APATCCharacter* WidgetOwner);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StopMinigame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartTreatmentAnim();
	
	FDelegateHandle StartTreatmentDelegate;
	
public:
	
	UFUNCTION()
	void RegisterPatient(APATCPatient* NewPatient);
	
	UFUNCTION()
	void UnregisterPatient();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void ProcessImmediateTreatment();
};
