// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

DECLARE_LOG_CATEGORY_EXTERN(LogUtilityAiEditor, Log, All);

struct FComboTest
{
	FText Label;
};

class FUtilityAiEditorModule : public IModuleInterface
{
private:
	TSharedPtr<class FExtender> mpBTMenuExtender;
	TSharedPtr<class FUICommandList> ToolbarCommands;
	TArray<TSharedPtr< FComboTest>> Options;
	TSharedPtr<FComboTest> CurrentItem;
	TSharedPtr<STextBlock> CurrentText;

	/** This function will be bound to Command. */
	void PluginButtonClicked();
	void UpdateProperty();

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
