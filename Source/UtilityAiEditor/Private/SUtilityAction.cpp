#include "SUtilityAction.h"
#include "UtilityAiEditor.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "SBlackboardEntryInfo.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityAction"

const FText SUtilityAction::TextCreateActionLabel = LOCTEXT("UtilityTreeWizard_CreateAction", "Create Action");
const FText SUtilityAction::TextLabelBlackboardKey = LOCTEXT("UtilityTreeWizard_BlackboardKey", "Blackboard Key");

void SUtilityAction::Construct(const FArguments& InArgs)
{
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Action: %i"), this->mpBlackboard.IsValid());

	int32 index = InArgs._Index.Get();
	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock).Text(SUtilityAction::TextCreateActionLabel)
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.Text(TextLabelBlackboardKey)
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SBlackboardEntryInfo)
				.BlackboardAsset(this->mpBlackboard)
			]
	];
}

#undef LOCTEXT_NAMESPACE