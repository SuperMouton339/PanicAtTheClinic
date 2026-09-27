// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanicClinic_EIK/Interfaces/PATCHealthEvents.h"
#include "PATCBaseEntity.generated.h"

class UPATCHealthComponent;

UCLASS()
class PANICCLINIC_EIK_API APATCBaseEntity : public APawn, public IPATCHealthEvents
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCBaseEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UPATCHealthComponent> HealthComponent;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerDeathAnim();
	virtual void MulticastTriggerDeathAnim_Implementation();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerHitFX();
	virtual void MulticastTriggerHitFX_Implementation();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerHealFX();
	virtual void MulticastTriggerHealFX_Implementation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathAnim();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitFX();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHealFX();
	
	//This is here to replace an animation.
	FTimerHandle DeathTimerHandle;
	
	UFUNCTION()
	void KillEntity();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnDeath_Implementation() override;
	
	virtual void OnDamageTaken_Implementation() override;
	
	virtual void OnHealthReceived_Implementation() override;
};
