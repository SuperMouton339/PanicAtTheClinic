// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PATCBaseEntity.h"
#include "PATCZombie.generated.h"

UCLASS()
class PANICCLINIC_EIK_API APATCZombie : public APATCBaseEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
