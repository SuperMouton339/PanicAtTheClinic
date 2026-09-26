// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCObjectSpawner.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "PanicClinic_EIK/EquippableItems/PATCConsumable.h"

// Sets default values
APATCObjectSpawner::APATCObjectSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void APATCObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
	//This is here for testing purposes.
	/*
	if (HasAuthority())
	{
		EndTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds() + CooldownDuration;
		bOnCooldown = true;
		
		UE_LOG(LogTemp, Warning, TEXT("Cooldown Started."));
	}*/
}

void APATCObjectSpawner::OnRep_EndTime()
{
	bOnCooldown = true;
}

void APATCObjectSpawner::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APATCObjectSpawner, EndTime);
}

// Called every frame
void APATCObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bOnCooldown)
	{
		RemainingTime = EndTime - GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		
		if (RemainingTime <= 0.0f)
		{
			bOnCooldown = false;
			UE_LOG(LogTemp, Warning, TEXT("Cooldown ended"));
		}
	}
}

void APATCObjectSpawner::OnInteracted_Implementation(ACharacter* InstigatorCharacter)
{
	if (bOnCooldown) return;
	
	if (!InstigatorCharacter->IsLocallyControlled()) return;
	
	//TODO - Pop selection UI for local controller that interacted with this.
}

void APATCObjectSpawner::ServerSpawnConsumable_Implementation(TSubclassOf<AActor> ObjectToSpawn)
{

	AActor* SpawnedObject = NewObject<AActor>(this, ObjectToSpawn->StaticClass(), MakeUniqueObjectName(this, ObjectToSpawn, FName(*FString("Equippable_"))));

	if (!HasAuthority()) return;
	
	EndTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds() + CooldownDuration;
	bOnCooldown = true;
	
	//TODO - Make this actually viable
	SpawnedObject->SetActorLocation(this->GetActorLocation());
}

