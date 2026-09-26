// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PATCCharacter.generated.h"



//Forward declaration
class UInputAction;
struct FInputActionValue;
class UPATCInteractionDetector;

UCLASS()
class PANICCLINIC_EIK_API APATCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APATCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Movement Declaration **/
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& Value);
	
	/** Temp Ground **/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPATCInteractionDetector> InteractionDetector;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerInteract(AActor* ActorToInteract);
	
	void ServerInteract_Implementation(AActor* ActorToInteract);
	
	UPROPERTY(BlueprintReadOnly, Category="Equipment", ReplicatedUsing=OnRep_EquippedItem)
	AActor* EquippedItem;
	
	UFUNCTION()
	void OnRep_EquippedItem();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Server, Reliable)
	void ServerEquipNewItem(AActor* ActorToEquip);
	
	void ServerEquipNewItem_Implementation(AActor* ActorToEquip);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
