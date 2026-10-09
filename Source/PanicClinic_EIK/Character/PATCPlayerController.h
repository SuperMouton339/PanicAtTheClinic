// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PATCPlayerController.generated.h"

/**
 * 
 */


//Forward Declare
class UInputMappingContext;
class APATCMinigame;

UCLASS()
class PANICCLINIC_EIK_API APATCPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//Constructor: for now used to plug our custom camera manager
	APATCPlayerController();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(ReplicatedUsing=OnRep_ActiveMinigame, BlueprintReadOnly, Category="Minigames")
	TObjectPtr<APATCMinigame> ActiveMinigameInstance;
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> MinigameMappingContext;
	
	UFUNCTION()
	void OnRep_ActiveMinigame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void RegisterNewMinigame();
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetActiveMinigame(APATCMinigame* NewMinigame)
	{
		ActiveMinigameInstance = NewMinigame;
		OnRep_ActiveMinigame();
	}
	
	UFUNCTION(BlueprintCallable)
	void StopActiveMinigame();
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
