#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#include "BehaviorTree/BlackboardData.h"
#include "Widgets/SBoxPanel.h"

class SBlackboardEntryInfo
	: public SCompoundWidget
{
	TSharedPtr<UBlackboardData> mpBlackboard;

	bool mIsCreatingEntry;
	TSharedPtr<SVerticalBox> mpWidgetEntryVars;

	void OnClearEntrySelection();
	void OnMakeEntrySelection(FBlackboardEntry entry);
	void OnSelectKeyType(UClass* InClass);

public:

	SLATE_BEGIN_ARGS(SBlackboardEntryInfo)
		: _BlackboardAsset(nullptr)
	{}

		SLATE_ATTRIBUTE(TSharedPtr<UBlackboardData>, BlackboardAsset)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};

