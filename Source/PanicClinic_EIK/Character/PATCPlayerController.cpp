// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PATCCameraManager.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Minigames/PATCMinigame.h"

APATCPlayerController::APATCPlayerController()
{
	// Every PlayerController (server copies included) spawns an APATCCameraManager,
	// but only locally controlled ones compute the view (see UpdateCamera / bUseClientSideCameraUpdates
	PlayerCameraManagerClass = APATCCameraManager::StaticClass();
}

void APATCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//Guard if the instance doesn't have a LocalPlayer (Server)
	if (!IsLocalController())
	{
		return;
	}
	
	//If there's a DefaultMappingContext to begin with
	if (DefaultMappingContext)
	{
		//If LP not nullptr (Not a NPC)
		if (ULocalPlayer* LP= GetLocalPlayer())
		{
			//If we can have a reference of the LocalPlayer's EnhancedInput
			if (UEnhancedInputLocalPlayerSubsystem* EILPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
			{
        			EILPlayerSubsystem->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
	
}

void APATCPlayerController::OnRep_ActiveMinigame()
{
	if (!IsLocalController()) return;
	
	if (ActiveMinigameInstance == nullptr) return;
	
	if (ActiveMinigameInstance->GetInputMappingContext() == nullptr) return;
		
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) return;
		
	UEnhancedInputLocalPlayerSubsystem* EILPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
	if (!EILPlayerSubsystem) return;
		
	MinigameMappingContext = ActiveMinigameInstance->GetInputMappingContext();
	EILPlayerSubsystem->AddMappingContext(MinigameMappingContext,0);
		
	EILPlayerSubsystem->RemoveMappingContext(DefaultMappingContext);
	
	RegisterNewMinigame();
	
}

void APATCPlayerController::StopActiveMinigame()
{
	
	if (!IsLocalController()) return;
	
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) return;
		
	UEnhancedInputLocalPlayerSubsystem* EILPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
	if (!EILPlayerSubsystem) return;
		

	EILPlayerSubsystem->AddMappingContext(DefaultMappingContext,0);
		
	//Kept this like that instead of checking for nullptr because it makes the function easier to extend later on.
	if (MinigameMappingContext != nullptr)
	{
		EILPlayerSubsystem->RemoveMappingContext(MinigameMappingContext);
	}
}

void APATCPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APATCPlayerController, ActiveMinigameInstance);
}
