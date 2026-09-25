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

};
