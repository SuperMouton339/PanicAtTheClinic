// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PATCCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PANICCLINIC_EIK_API APATCCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="CameraZoom")
	float MaxZoomDistance = 2000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = 5.f), Category="CameraZoom")
	float MinZoomDistance = 200.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = 20.f, ClampMax= 90.f), Category="CameraZoom")
	float CameraAngle = 45.f;
	
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
private:
	float CalculateCameraZoom();
	
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
