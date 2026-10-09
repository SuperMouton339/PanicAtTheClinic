// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PanicClinic_EIK/Interfaces/PATCHealthEvents.h"
#include "GameFramework/Character.h"
#include "PanicClinic_EIK/Interfaces/PATCInteractable.h"
#include "PATCCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPATCNetCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsDownChanged, bool, bIsDown);

//Forward declaration
class UInputAction;
struct FInputActionValue;
class UPATCInteractionDetector;
class UCharacterMovementComponent;
class UPATCHealthComponent;
class USphereComponent;

UCLASS()
class PANICCLINIC_EIK_API APATCCharacter : public ACharacter, public IPATCHealthEvents, public IPATCInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APATCCharacter();

	
	UPROPERTY(BlueprintAssignable)
	FOnIsDownChanged OnIsDownChanged;
	
	// Online Multiplayer PlayerState
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnRefreshPlayerNumber(int32 PlayerNumber);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Online Multiplayer Tools Declaration
	void RefreshPlayerNumber();
	
	/** Movement Declaration **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComp;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	float RotationRate = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment",meta=(ToolTip="Value in centimeter 100 = 1 meter"))
	float DropItemLength = 100.0f;
	
	
	void Move(const FInputActionValue& Value);
	
	/** Interactable Section   **/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPATCInteractionDetector> InteractionDetector;
	
	// For Reviving down player purposes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> ReviveInteractionZone;
	
	virtual void OnInteracted_Implementation(ACharacter* InstigatorCharacter) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(ClampMin="1"))
	int32 HealAmountOnRevive = 1;
	
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerInteract(AActor* ActorToInteract);
	void ServerInteract_Implementation(AActor* ActorToInteract);
	
	
	// Use entry points (client -> server). No parameter on purpose: the server uses its own EquippedItem,
	// the client never tells the server which item it holds.
	// Reliable: rare discrete events (press/release); a lost Stop would leave a use running forever.
	// Input binding rule (C++ OR BP, never both, or each press sends two RPCs):
	//   Started -> ServerStartUse, Completed AND Canceled -> ServerStopUse. Never Triggered (RPC every frame).
	
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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEquippedChanged();

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bHasWeapon = false;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDropItem();
	void ServerDropItem_Implementation();
	
	
	/***  **/
	
	/** Attack Section **/
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Attack")
	void ServerAttack();
	void ServerAttack_Implementation();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Attack")
	void ProcessAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerAttackAnim();
	void MulticastTriggerAttackAnim_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category="Attack", meta=(ToolTip="Should be used to enable an attack animation, is already replicated in C++ calls."))
	void StartAttackAnim();
	
	/**   **/
	
	/** Health Section **/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IsDowned)
	bool bIsDowned = false;
	
	
	void SetIsDowned(bool bInState);
	
	UFUNCTION()
	void OnRep_IsDowned();
	
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UPATCHealthComponent> PlayerHealthComponent;
	
	/**   **/
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
	
	// Server-side
	UFUNCTION()
	void DropItemSequence();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintPure)
	AActor* GetEquippedItem() const
	{
		return EquippedItem;
	}

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTriggerTreatmentFX(bool bInState);

	UFUNCTION()
	void MulticastTriggerTreatmentFX_Implementation(bool bInState);

	UFUNCTION(BlueprintImplementableEvent)
	void ProcessTreatmentFX(bool bInState);
	
	/** Public Health Section **/
	
	
	UFUNCTION(BlueprintCallable)
	bool IsDowned() const;
	
	UFUNCTION(BlueprintCallable)
	UPATCHealthComponent* GetPlayerHealthComponent() const;
	
	virtual void OnHealthReceived_Implementation() override;
	virtual void OnDeath_Implementation() override;
	virtual void OnDamageTaken_Implementation() override;


private:
	void ApplyMovementSettings();
};
