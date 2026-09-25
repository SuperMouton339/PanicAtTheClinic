#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PATCDeath.generated.h"

UINTERFACE(Blueprintable)
class UPATCDeath : public UInterface
{
public:
	GENERATED_BODY()
	
};

class IPATCDeath
{
public:

	GENERATED_BODY()
	//Gentleman's agreement to not override this in blueprint PLEASE
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeath();
};
