// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PATCPlayerState.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogPATCNetPlayerState, Log, All);
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNumberAssigned, int32, PlayerNumber);

UCLASS()
class PANICCLINIC_EIK_API APATCPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	APATCPlayerState();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerNumberAssigned OnPlayerNumberAssigned;
	
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerNumber() const;

	void SetPlayerNumber(int32 PlayerNumberAssignation);
	
protected:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PlayerNumber)
	int32 PlayerNumber = 0;
	
	UFUNCTION()
	void OnRep_PlayerNumber();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
