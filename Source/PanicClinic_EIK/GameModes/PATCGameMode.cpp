// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCGameMode.h"

#include "PATCGameState.h"
#include "PATCPlayerState.h"

DEFINE_LOG_CATEGORY(LogPATCNetGameMode);

void APATCGameMode::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);
	
	APATCPlayerState* SomePlayerState = Cast<APATCPlayerState>(C->PlayerState);
	if (!IsValid(SomePlayerState)) return; 
	
	if (SomePlayerState->GetPlayerNumber() > 0) return;
	
	
	AGameStateBase* GS = GetGameState<AGameStateBase>();
	if (!IsValid(GS)) return;
	
	const TArray<TObjectPtr<APlayerState>>& PArray = GS->PlayerArray;
	
	
	
	// Array of numbers already assignated
	TArray<int32> ArrayOfNumPlayers;
	
	for (APlayerState* PlayerState : PArray )
	{
		if (APATCPlayerState* PATCPlayerState = Cast<APATCPlayerState>(PlayerState))
		{
			
			if (PATCPlayerState->GetPlayerNumber() > 0 )
			{
				ArrayOfNumPlayers.Add(PATCPlayerState->GetPlayerNumber());
			}
		}
	}
	
	for (int32 i = 1; i <= PArray.Num(); i++)
	{
		if (ArrayOfNumPlayers.Contains(i)) continue;
		
		
		SomePlayerState->SetPlayerNumber(i);
		
		break;
		
	}
	UE_LOG(LogPATCNetGameMode, Log, TEXT("Controller Name: %s, Player number: %i"), *C->GetName(), SomePlayerState->GetPlayerNumber());
}
