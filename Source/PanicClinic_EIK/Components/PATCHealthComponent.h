// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PATCHealthComponent.generated.h"

/*
 * Overall this class has a few consistency issues that should be ironed out post technical proof:
 * 1- Death is handled through an interface while health change notification goes through a delegate.
 * 2- Health is replicated but for an actor to know that their health changed they need to be manually notified, this should not be needed. This makes hit handling pretty damn messy.
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PANICCLINIC_EIK_API UPATCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPATCHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void TakeDamage(int DamageAmount);

	UFUNCTION()
	void GainHealth(int HealAmount);
	
	UFUNCTION(BlueprintCallable)
	int GetCurrentHealth()
	{
		return CurrentHealth;
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(ReplicatedUsing=OnRep_Health)
	int CurrentHealth = 5;

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditDefaultsOnly, Category="Health")
	int MaxHealth = 5;
	
	bool bImplementsHealthInterface = false;
};
