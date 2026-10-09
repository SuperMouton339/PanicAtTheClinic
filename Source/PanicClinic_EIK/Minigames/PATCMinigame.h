// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PATCMinigame.generated.h"

class APATCPatient;
class APATCCharacter;
class UInputMappingContext;

UCLASS()
class PANICCLINIC_EIK_API APATCMinigame : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCMinigame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Minigame")
	TObjectPtr<UInputMappingContext> GrantedMappingContext;
	
	UPROPERTY(BlueprintReadOnly, Category="Minigame")
	TObjectPtr<APATCPatient> BoundPatient;

	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMinigameWidget();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginMinigame(APATCCharacter* MinigameParticipant, APATCPatient* Patient);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopMinigame();
	
	UFUNCTION(BlueprintCallable)
	UInputMappingContext* GetInputMappingContext() const
	{
		return GrantedMappingContext;
	}
};
