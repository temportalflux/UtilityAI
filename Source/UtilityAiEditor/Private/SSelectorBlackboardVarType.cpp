#include "SSelectorBlackboardVarType.h"

#include "Widgets/Layout/SBox.h"

#include "Framework/Application/SlateApplication.h"
#include "Modules/ModuleManager.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"

// NOTE: A lot of this is duplicate code from SBehaviorTreeBlackboardEditor.cpp

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SSelectorBlackboardKeyType"

class FBlackboardEntryClassFilter : public IClassViewerFilter
{
public:

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		if (InClass != nullptr)
		{
			return !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_HideDropDown) &&
				InClass->HasAnyClassFlags(CLASS_EditInlineNew) &&
				InClass->IsChildOf(UBlackboardKeyType::StaticClass());
		}
		return false;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return InUnloadedClassData->IsChildOf(UBlackboardKeyType::StaticClass());
	}
};

TSharedRef<SWidget> SSelectorBlackboardVarType::BuildSelectMenu()
{
	FClassViewerInitializationOptions Options;
	Options.bShowUnloadedBlueprints = true;
	Options.bShowDisplayNames = true;
	Options.ClassFilter = MakeShareable(new FBlackboardEntryClassFilter);

	return
		SNew(SBox)
		.HeightOverride(240.0f)
		.WidthOverride(200.0f)
		[
			FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(
				Options,
				FOnClassPicked::CreateRaw(this, &SSelectorBlackboardVarType::HandleKeyClassPicked)
			)
		];
}

void SSelectorBlackboardVarType::HandleKeyClassPicked(UClass * InClass)
{
	FSlateApplication::Get().DismissAllMenus();

	check(InClass);
	check(InClass->IsChildOf(UBlackboardKeyType::StaticClass()));

	if (this->OnSelected.IsBound())
		this->OnSelected.Execute(InClass);
}

void SSelectorBlackboardVarType::Construct(const FArguments & InArgs)
{
	this->SetOnSelected(InArgs._OnSelected);

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SAssignNew(mpSelector, SComboButton)
			.OnGetMenuContent(FOnGetContent::CreateRaw(this,
				&SSelectorBlackboardVarType::BuildSelectMenu))
			.ContentPadding(FMargin(2.0f, 2.0f))
			.MenuPlacement(MenuPlacement_BelowAnchor)
			.ButtonContent()
			[
				SAssignNew(mpLabel, STextBlock)
				.Text(LOCTEXT("Prompt", "Select type"))
			]
		]

	];
}

void SSelectorBlackboardVarType::SetOnSelected(FOnSelectedBlackboardVarType InDelegate)
{
	OnSelected = InDelegate;
}

#undef LOCTEXT_NAMESPACE
