// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PATCCameraManager.h"

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
