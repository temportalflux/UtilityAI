// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

DECLARE_LOG_CATEGORY_EXTERN(LogUtilityAiEditor, Log, All);

class FUtilityAiEditorModule : public IModuleInterface
{
private:
	TSharedPtr<class FExtender> mpBTMenuExtender;
	TSharedPtr<class FExtender> mpBTToolbarExtender;
	TSharedPtr<class FUICommandList> ToolbarCommands;

	void AddBTEditorToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	/** This function will be bound to Command. */
	void PluginButtonClicked();

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
