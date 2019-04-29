#include "SWizardSidebar.h"
#include "UtilityAiEditor.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SComboButton.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SWizardSidebar"

void SWizardSidebar::Construct(const FArguments& InArgs)
{
	this->mOnAddAction = InArgs._OnAddAction;
	this->mOnEditAction = InArgs._OnEditAction;
	this->mfGetUnusedAction = InArgs._GetUnusedAction;

	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.AutoWidth()
					.Padding(10.0f)
					[
						SAssignNew(mpButtonFinish, SButton)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1, 0, 0)))
						.OnPressed(InArgs._OnFinish)
						.Content()
						[ SNew(STextBlock).Text(LOCTEXT("SWizardSidebar_ButtonFinish", "Finish")) ]
					]

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.AutoWidth()
					.Padding(10.0f)
					[
						SAssignNew(mpButtonAddAction, SButton)
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SWizardSidebar::AddAction))
						.Content()
						[ SNew(STextBlock).Text(LOCTEXT("SWizardSidebar_ButtonAddAction", "+ Action")) ]
					]
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			.Padding(10.0f)
			[
				SAssignNew(mpActionListingsPanel, SVerticalBox)
			]

	];
}

void SWizardSidebar::AddAction()
{
	TSharedRef<FUtilityActionDetails> action =
		this->mfGetUnusedAction.IsBound()
		? this->mfGetUnusedAction.Execute()
		: MakeShared<FUtilityActionDetails>();

	if (!this->mfGetUnusedAction.IsBound())
	{
		action->mIndex = this->mpActionListingsPanel->GetChildren()->Num();
		action->mShouldGenerate = true;
	}

	action->mName = TEXT("Action");
	action->mInputs.Empty();

	if (action->mShouldGenerate)
	{
		this->mpActionListingsPanel->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
		[
			SNew(SActionListing)
			.Action(TWeakPtr<FUtilityActionDetails>(action))
			// Delegate from button requesting to open the edit panel
			.OnEdit(mOnEditAction)
		];
	}

	this->mOnAddAction.ExecuteIfBound(action);
}

#undef LOCTEXT_NAMESPACE