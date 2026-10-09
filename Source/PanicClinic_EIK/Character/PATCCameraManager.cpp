// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCCameraManager.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "DrawDebugHelpers.h"


void APATCCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{	
	//Super::UpdateViewTarget(OutVT, DeltaTime);
	
	TArray<FVector> PlayersLocations;
	
	const AGameStateBase* GameState = GetWorld()->GetGameState();
	if (GameState == nullptr)
	{
		return;
	}
	for (const APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (PlayerState == nullptr )
		{
			continue;
		}
		
		if (const APawn* PlayerPawn = PlayerState->GetPawn())
		{
			PlayersLocations.Add(PlayerPawn->GetActorLocation());
		}
	}
	
	if (PlayersLocations.Num() == 0)
	{
		return;
	}
	
	FBox CameraBounds(PlayersLocations);
	
	const FVector CameraCenter = CameraBounds.GetCenter();
	//** FBox Test DrawDebug ***//
	DrawDebugBox(GetWorld(), CameraCenter, CameraBounds.GetExtent(), FColor::Green);
	DrawDebugSphere(GetWorld(), CameraCenter, 10, 10, FColor::Red);
	//***///
	
	
	//Put the POV Location in the middle of all players
	const FRotator CameraRotation = FRotator(-CameraAngle, 0.f, 0.f);
	// Start from the center and step back along the view direction by the zoom distance
	const FVector CameraLocation = CameraCenter - CameraRotation.Vector() * CalculateCameraZoom();
	OutVT.POV.Location = CameraLocation;

	// Pitch is -CameraAngle (Pitch, Yaw, Roll)
	OutVT.POV.Rotation = CameraRotation;
	
}

float APATCCameraManager::CalculateCameraZoom()
{
	return MaxZoomDistance;
}


#if WITH_EDITOR
void APATCCameraManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APATCCameraManager, MaxZoomDistance))
	{
		if (MaxZoomDistance < MinZoomDistance)
		{
			MaxZoomDistance = MinZoomDistance;
		}
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APATCCameraManager, MinZoomDistance))
	{
		if (MinZoomDistance > MaxZoomDistance)
		{
			MinZoomDistance = MaxZoomDistance;
		}
	}
}
#endif
