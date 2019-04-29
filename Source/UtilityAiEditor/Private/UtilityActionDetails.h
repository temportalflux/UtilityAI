#pragma once

#include "CoreMinimal.h"
#include "UtilityActionEntry.h"
#include "Framework/SlateDelegates.h"

struct FUtilityActionDetails;

DECLARE_DELEGATE_OneParam(FOnActionChanged, TWeakPtr<FUtilityActionDetails>)

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
	int32 mIndex;
	FName mName;
	TMap<FGuid, FUtilityActionInput> mInputs;
	FOnActionChanged mOnChanged;
	bool mShouldGenerate;
};
