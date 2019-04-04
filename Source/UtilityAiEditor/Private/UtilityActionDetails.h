#pragma once

#include "CoreMinimal.h"
#include "UtilityActionEntry.h"

struct FUtilityActionInputCurveKey
{
	float input;
	float output;
};

struct FUtilityActionInput
{
	FUtilityActionEntry mBlackboardKeyEntry;
	TMap<FGuid, FUtilityActionInputCurveKey> mCurveKeys;
};

struct FUtilityActionDetails
{
	FName mName;
	TMap<FGuid, FUtilityActionInput> mInputs;
};
