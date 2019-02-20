#include "SUtilityAction.h"
#include "UtilityAiEditor.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityAction"

const FText SUtilityAction::TextCreateActionLabel = LOCTEXT("UtilityTreeWizard_CreateAction", "Create Action");

void SUtilityAction::Construct(const FArguments& InArgs)
{
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
				SNew(STextBlock).Text(FText::FromString(FString::FromInt(index)))
			]
	];
}

#undef LOCTEXT_NAMESPACE