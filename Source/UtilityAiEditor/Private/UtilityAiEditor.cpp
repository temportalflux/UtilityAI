// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UtilityAiEditor.h"

//#include "Editor/BehaviorTreeEditor/Public/BehaviorTreeEditorModule.h"
//#include "BehaviorTreeEditor/Public/BehaviorTreeEditorModule.h"
#include "BehaviorTreeEditorModule.h"

DEFINE_LOG_CATEGORY(LogUtilityAiEditor);

#define LOCTEXT_NAMESPACE "FUtilityAiEditorModule"

void FUtilityAiEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Loading UtilityAiEditor..."));

	// https://answers.unrealengine.com/questions/25609/customizing-the-editors-toolbar-buttons-menu-via-c.html

	TSharedPtr<FExtender> extender = MakeShareable(new FExtender);
	mpBTToolbarExtender = extender;
	mpBTToolbarExtensionUtility = mpBTToolbarExtender->AddToolBarExtension(
		"BehaviorTree",
		EExtensionHook::After,
		NULL,
		FToolBarExtensionDelegate::CreateRaw(this, &FUtilityAiEditorModule::AddBTEditorToolbarExtension)
	);

	FBehaviorTreeEditorModule& BTEditorModule = FModuleManager::LoadModuleChecked<FBehaviorTreeEditorModule>("BehaviorTreeEditor");
	BTEditorModule.GetToolBarExtensibilityManager()->AddExtender(mpBTToolbarExtender);
}

void FUtilityAiEditorModule::ShutdownModule()
{
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Unloading UtilityAiEditor..."));
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FBehaviorTreeEditorModule& BTEditorModule = FModuleManager::LoadModuleChecked<FBehaviorTreeEditorModule>("BehaviorTreeEditor");
	//this->mpBTToolbarExtender->RemoveExtension(mpBTToolbarExtensionUtility);
	BTEditorModule.GetToolBarExtensibilityManager()->RemoveExtender(this->mpBTToolbarExtender);
}

void FUtilityAiEditorModule::AddBTEditorToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddSeparator();
	UE_LOG(LogUtilityAiEditor, Log, TEXT("BT Utiltiy toolbar button time"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAiEditorModule, UtilityAiEditor)