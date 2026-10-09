// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PATCBaseEquippable.h"
#include "GameFramework/Actor.h"
#include "PanicClinic_EIK/Interfaces/PATCEquippableItem.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PanicClinic_EIK/Interfaces/PATCWeapon.h"
#include "PATCAmputationTool.generated.h"

UCLASS()
class PANICCLINIC_EIK_API APATCAmputationTool : public APATCBaseEquippable
	, public IPATCWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCAmputationTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteracted_Implementation(ACharacter* InstigatorCharacter) override;
	virtual void OnUseStarted_Implementation(ACharacter* InstigatorCharacter) override;
	virtual void OnUseStopped_Implementation(ACharacter* InstigatorCharacter) override
	{
		return;
	}

	virtual void OnAttack_Implementation(ACharacter* InstigatorCharacter) override
	{
		return;
	}
};
