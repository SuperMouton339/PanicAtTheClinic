#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PATCInteractable.generated.h"

UINTERFACE(Blueprintable, MinimalAPI)
class UPATCInteractable : public UInterface
{
	GENERATED_BODY()
};

class IPATCInteractable
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	PANICCLINIC_EIK_API virtual void OnInteracted(APlayerController* InstigatorPC) = 0;
};