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
	
	/** ***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPATCInteractionDetector> InteractionDetector;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerInteract(AActor* ActorToInteract);
	void ServerInteract_Implementation(AActor* ActorToInteract);
	
	// Use entry points (client -> server). No parameter on purpose: the server uses its own EquippedItem,
	// the client never tells the server which item it holds.
	// Reliable: rare discrete events (press/release); a lost Stop would leave a use running forever.
	// Input binding rule (C++ OR BP, never both, or each press sends two RPCs):
	//   Started -> ServerStartUse, Completed AND Canceled -> ServerStopUse. Never Triggered (RPC every frame).
	// TODO - Team decision: bind Interact/Use in C++ or BP (currently BP, temporary).
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerStartUse();
	void ServerStartUse_Implementation();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerStopUse();
	void ServerStopUse_Implementation();
	
	
	UPROPERTY(BlueprintReadOnly, Category="Equipment", ReplicatedUsing=OnRep_EquippedItem)
	AActor* EquippedItem;
	
	UFUNCTION()
	void OnRep_EquippedItem();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Server-side equip. NOT an RPC: only called by server code (e.g. OnInteracted),
	// so clients have no network entry point to equip arbitrary items.
	// Returns false if the equip was refused, so the caller can leave the item in the world.
	UFUNCTION()
	bool EquipNewItem(AActor* ActorToEquip);
	
	// Server-side unequip, mirror of EquipNewItem. NOT an RPC: only called by server code (e.g. consumed item).
	UFUNCTION()
	void UnequipItem();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
