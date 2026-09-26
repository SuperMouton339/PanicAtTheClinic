// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "PATCInteractionDetector.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PANICCLINIC_EIK_API UPATCInteractionDetector : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPATCInteractionDetector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	/*
	 *	Server functions.
	 */
	
	UFUNCTION(Server, Reliable)
	void ServerAddToInteractionList(AActor* ActorToAdd);
	
	void ServerAddToInteractionList_Implementation(AActor* ActorToAdd);
	
	UFUNCTION(Server, Reliable)
	void ServerRemoveFromInteractionList(AActor* ActorToRemove);
	
	void ServerRemoveFromInteractionList_Implementation(AActor* ActorToRemove);
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
		
	UFUNCTION()
	void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	UFUNCTION()
	void OnRep_InteractionList();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetInteractionList()
	{
		return InteractionList;
	}

protected:
	
	UPROPERTY(ReplicatedUsing=OnRep_InteractionList, BlueprintReadOnly)
	TArray<AActor*> InteractionList;
};
