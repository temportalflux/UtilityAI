#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class SUtilityAction
	: public SCompoundWidget
{
	static const FText TextCreateActionLabel;

public:

	SLATE_BEGIN_ARGS(SUtilityAction)
		: _Index(0)
	{}

		SLATE_ATTRIBUTE(int32, Index)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
