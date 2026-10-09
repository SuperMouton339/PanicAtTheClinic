// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCAmputationTool.h"


// Sets default values
APATCAmputationTool::APATCAmputationTool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APATCAmputationTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APATCAmputationTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APATCAmputationTool::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	Super::OnInteracted_Implementation(InstigatorCharacter);
}

void APATCAmputationTool::OnUseStarted_Implementation(ACharacter* InstigatorCharacter)
{
	Super::OnUseStarted_Implementation(InstigatorCharacter);
}

