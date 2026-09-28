#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PATCEquippableItem.generated.h"

UINTERFACE(Blueprintable, MinimalAPI)
class UPATCEquippableItem : public UInterface
{
	GENERATED_BODY()
};

class IPATCEquippableItem
{
	GENERATED_BODY()

public:
	//I'm personally not the biggest fan of how high up the hierarchy the handshake happens on this, but we'll see how it goes. - Jacob
	//Though I read online that it is indeed the way to use interfaces in this context, might just be my preference for component-based approaches talking. - Jacob
	
	// Named after the game action, NOT the input event: the server may also stop a use
	// with no input at all (player hit by a zombie, patient turns).
	// Always called on the server by APATCCharacter::ServerStartUse / ServerStopUse.
	// Item-specific rules (patient in range, ammo...) are validated HERE, not in the Character:
	// the Character RPCs are shared by every equippable (consumables AND weapons).
	UFUNCTION(BlueprintNativeEvent)
	void OnUseStarted(ACharacter* InstigatorCharacter);
	
	// May be called without a matching OnUseStarted (e.g. item equipped while the key was held): must be safe to call anytime.
	UFUNCTION(BlueprintNativeEvent)
	void OnUseStopped(ACharacter* InstigatorCharacter);
};