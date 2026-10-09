// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCPlayerState.h"

#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogPATCNetPlayerState);
APATCPlayerState::APATCPlayerState()
{
	
}

int32 APATCPlayerState::GetPlayerNumber() const
{
	return PlayerNumber;
}

void APATCPlayerState::OnRep_PlayerNumber()
{
	UE_LOG(LogPATCNetPlayerState, Log, TEXT("HasAuthority? : %s, Player State Name: %s, Player Number: %d"), 
		(HasAuthority() ? TEXT("True") : TEXT("False")), *GetName(), PlayerNumber);
	
	
	OnPlayerNumberAssigned.Broadcast(PlayerNumber);
}

void APATCPlayerState::SetPlayerNumber(int32 PlayerNumberAssignation)
{
	if (PlayerNumber == PlayerNumberAssignation) return;
	if (!HasAuthority()) return;
	
	PlayerNumber = PlayerNumberAssignation;

	OnRep_PlayerNumber();
}

void APATCPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APATCPlayerState, PlayerNumber);
	
}
