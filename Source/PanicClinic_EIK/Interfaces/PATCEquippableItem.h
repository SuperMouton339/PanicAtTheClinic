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
	
	
	UFUNCTION()
	PANICCLINIC_EIK_API virtual void OnStarted(APlayerController* InstigatorPC) = 0;
	
	UFUNCTION()
	PANICCLINIC_EIK_API virtual void OnTriggered(APlayerController* InstigatorPC) = 0;
};