#pragma once

#include "CoreMinimal.h"
#include "UtilityActionEntry.h"

struct FUtilityActionInput
{
	FUtilityActionEntry mBlackboardKeyEntry;
	float mInputValue;

};

struct FUtilityActionDetails
{
	FName mName;
	TMap<FGuid, FUtilityActionInput> mInputs;
};
