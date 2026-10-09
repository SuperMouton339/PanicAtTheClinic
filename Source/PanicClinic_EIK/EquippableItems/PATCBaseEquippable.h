// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanicClinic_EIK/Interfaces/PATCEquippableItem.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PATCBaseEquippable.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS(Blueprintable)
class PANICCLINIC_EIK_API APATCBaseEquippable : public AActor, public IPATCInteractable, public IPATCEquippableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APATCBaseEquippable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMeshComponent* ItemStaticMesh;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USceneComponent* RootSceneComponent;
	
	//Reference stocked on BeginPlay only on Server side to put back the picked up item at the same rotation
	UPROPERTY(BlueprintReadOnly)
	FRotator ItemInitialRotation;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteracted_Implementation(ACharacter* InstigatorCharacter) override;
	virtual void OnDropped_Implementation(FVector DroppedPosition) override;
	
	// Called on clients when Owner replicates; called manually on the server after SetOwner.
	// Owned (held) items must not be detectable: collision off. Dropped items: collision back on.
	virtual void OnRep_Owner() override;
};
