// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCGameState.h"

#include "PATCPlayerState.h"

APATCPlayerState* APATCGameState::FindPlayerStateByNumber(int32 PlayerNumber) const
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		APATCPlayerState* PatcPlayerState = Cast<APATCPlayerState>(PlayerState);
		if (IsValid(PatcPlayerState) && PatcPlayerState->GetPlayerNumber() == PlayerNumber)
		{
			return PatcPlayerState;
		}
	}
	return nullptr;
}
