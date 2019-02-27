#pragma once

#include "CoreMinimal.h"
#include "UtilityActionEntry.h"

struct FUtilityActionInput
{
	FUtilityActionEntry mBlackboardKeyEntry;
};

struct FUtilityActionDetails
{
	FName mName;
	TArray<FUtilityActionInput> mInputs;
};
