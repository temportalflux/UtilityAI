#include "SUtilityAction.h"
#include "UtilityAiEditor.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "SUtilityActionInput.h"

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
			.AutoHeight()
			[
				SNew(STextBlock).Text(SUtilityAction::TextCreateActionLabel)
			]
	
			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("AddEntry", "Create input"))
				.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityAction::AddInputField))
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SAssignNew(mpActionInputsBox, SVerticalBox)
					]
			]

	];
}

void SUtilityAction::AddInputField()
{
	this->mpActionInputsBox->AddSlot()
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(TextLabelBlackboardKey)
					]
			
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SNew(SButton)
						.Text(LOCTEXT("DeleteEntry", "Remove"))
						.OnPressed(FSimpleDelegate::CreateLambda([]() {
					
						}))
					]
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SUtilityActionInput)
				.BlackboardAsset(this->mpBlackboard)
			]

	];
}

#undef LOCTEXT_NAMESPACE