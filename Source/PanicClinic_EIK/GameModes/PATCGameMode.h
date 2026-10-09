// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PATCGameMode.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogPATCNetGameMode, Log, All);

UCLASS()
class PANICCLINIC_EIK_API APATCGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	virtual void GenericPlayerInitialization(AController* C) override;
};
