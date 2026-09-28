#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PATCHealthEvents.generated.h"

UINTERFACE(Blueprintable)
class UPATCHealthEvents : public UInterface
{
public:
	GENERATED_BODY()
	
};

class IPATCHealthEvents
{
public:

	GENERATED_BODY()
	//Gentleman's agreement to not override this in blueprint without asking beforehand
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeath();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDamageTaken();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHealthReceived();
};
