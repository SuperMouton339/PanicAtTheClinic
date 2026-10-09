// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PATCGameState.generated.h"

class APATCPlayerState;

/**
 * 
 */
UCLASS()
class PANICCLINIC_EIK_API APATCGameState : public AGameState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="Players")
	APATCPlayerState* FindPlayerStateByNumber(int32 PlayerNumber) const;
};
