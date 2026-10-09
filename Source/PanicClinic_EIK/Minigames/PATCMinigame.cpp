// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCMinigame.h"

#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/Entities/PATCPatient.h"


// Sets default values
APATCMinigame::APATCMinigame()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void APATCMinigame::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APATCMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APATCMinigame::StopMinigame_Implementation()
{
}

void APATCMinigame::BeginMinigame_Implementation(APATCCharacter* MinigameParticipant, APATCPatient* Patient)
{
	BoundPatient = Patient;
}

