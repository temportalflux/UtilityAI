#include "SUtilityActionInput.h"

#include "Widgets/SBoxPanel.h"
#include "SBlackboardEntryInfo.h"

void SUtilityActionInput::Construct(const FArguments & InArgs)
{
	this->mpBlackboard = InArgs._BlackboardAsset.Get();

	// TODO: Input value
	// TODO: Utility Value (probablity)
	//		low (25%), medium (50%), high (75%), definite (500%)
	// TODO: Curve Type
	//		constant, smooth, ???
	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SBlackboardEntryInfo)
				.BlackboardAsset(this->mpBlackboard)
				.OnChanged(FOnBlackboardEntryChanged::CreateLambda(
					[](FUtilityActionEntry const &entry)
					{
						
					}
				))
			]

			/*
			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(S)
			]
			//*/

	];
}
