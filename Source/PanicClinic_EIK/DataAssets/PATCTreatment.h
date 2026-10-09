// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PanicClinic_EIK/Enums/PATCTreatmentTypes.h"
#include "PATCTreatment.generated.h"

enum class EPATCTreatmentTypes : uint8;
/**
 * 
 */

USTRUCT(Blueprintable, BlueprintType)
struct PANICCLINIC_EIK_API FPATCMiniGameBinding
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class APATCBaseEquippable> RequiredEquippable;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class APATCMinigame> MiniGameForPlayer;
};

UCLASS(Blueprintable)
class PANICCLINIC_EIK_API UPATCTreatment : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Treatment Details")
	EPATCTreatmentTypes TreatmentType = EPATCTreatmentTypes::Simple;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Treatment Details")
	int BaseTreatmentReward = 0;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Visuals")
	TObjectPtr<class UTexture2D> WidgetImage;
	
	//The object that the player needs to have on him to do the treatment, can be left empty if the treatment does not require an object.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Simple Treatment", meta=(EditCondition="TreatmentType == EPATCTreatmentTypes::Simple", EditConditionHides, ToolTip="The object that the player needs to have on him to do the treatment, can be left empty if the treatment does not require an object."))
	TSubclassOf<class APATCBaseEquippable> RequiredEquippable;
	
	//The list of objects that are needed to do a complex treatment, spaces can be left empty for participants who do not need to have an item on them.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Complex Treatment", meta=(EditCondition="TreatmentType == EPATCTreatmentTypes::Complex", EditConditionHides, ToolTip="The list of objects that are needed to do a complex treatment, spaces can be left empty for participants who do not need to have an item on them."))
	TArray<FPATCMiniGameBinding> MiniGamesBindings;
};
