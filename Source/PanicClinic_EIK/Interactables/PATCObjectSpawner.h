// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PATCObjectSpawner.generated.h"

class APATCConsumable;

UCLASS()
class PANICCLINIC_EIK_API APATCObjectSpawner : public AActor, public IPATCInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCObjectSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(ReplicatedUsing = OnRep_EndTime)
	float EndTime;
	
	float RemainingTime;
	
	UFUNCTION()
	void OnRep_EndTime();
	
	UPROPERTY(EditDefaultsOnly, meta=(ToolTip="The duration is in seconds, so 60 = 1 min, 120 = 2 min, etc."))
	float CooldownDuration = 20.f;


public:
	// Called every frame
	
	UPROPERTY()
	bool bOnCooldown = false;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteracted_Implementation(ACharacter* InstigatorCharacter) override;
	
	UFUNCTION(NetMulticast, Reliable)
	void ServerSpawnConsumable(TSubclassOf<AActor> ObjectToSpawn);
	
	UPROPERTY(EditDefaultsOnly, meta=(MustImplement="/Script/PanicClinic_EIK.PATCInteractable"))
	TArray<TSubclassOf<AActor>> SpawnableClasses;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
