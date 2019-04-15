// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UtilityAiEditor.h"

#include "ToolbarStyle.h"
#include "ToolbarCommands.h"
#include "Misc/MessageDialog.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"
#include "DetailLayoutBuilder.h"

#include "UtilityTreeWizard.h"
#include "BehaviorTreeEditorModule.h"

DEFINE_LOG_CATEGORY(LogUtilityAiEditor);

#define LOCTEXT_NAMESPACE "FUtilityAiEditorModule"

void FUtilityAiEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Loading UtilityAiEditor..."));

	FToolbarStyle::Initialize();
	FToolbarStyle::ReloadTextures();

	FToolbarCommands::Register();

	ToolbarCommands = MakeShareable(new FUICommandList);
	mpUtilityTreeWizard = MakeShareable(new UtilityTreeWizard());

	ToolbarCommands->MapAction(
		FToolbarCommands::Get().Action_UtilityTree_OpenWizard,
		FExecuteAction::CreateRaw(mpUtilityTreeWizard.Get(), &UtilityTreeWizard::Open),
		FCanExecuteAction()
	);

	// https://answers.unrealengine.com/questions/25609/customizing-the-editors-toolbar-buttons-menu-via-c.html

	FBehaviorTreeEditorModule& BTEditorModule = FModuleManager::LoadModuleChecked<FBehaviorTreeEditorModule>("BehaviorTreeEditor");
	{
		this->mpBTMenuExtender = (TSharedPtr<FExtender>)MakeShareable(new FExtender());
		this->mpBTMenuExtender->AddMenuExtension(
			"WindowLayout",
			EExtensionHook::After,
			ToolbarCommands,
			FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& Builder) {
				Builder.AddMenuEntry(FToolbarCommands::Get().Action_UtilityTree_OpenWizard);
			})
		);
		BTEditorModule.GetMenuExtensibilityManager()->AddExtender(this->mpBTMenuExtender);
	}

}

void FUtilityAiEditorModule::ShutdownModule()
{
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Unloading UtilityAiEditor..."));
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FBehaviorTreeEditorModule& BTEditorModule = FModuleManager::LoadModuleChecked<FBehaviorTreeEditorModule>("BehaviorTreeEditor");
	BTEditorModule.GetMenuExtensibilityManager()->RemoveExtender(this->mpBTMenuExtender);

	FToolbarStyle::Shutdown();
	FToolbarCommands::Unregister();
	
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAiEditorModule, UtilityAiEditor)