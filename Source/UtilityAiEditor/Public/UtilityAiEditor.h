// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "MultiBoxExtender.h"
#include "MultiBoxBuilder.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUtilityAiEditor, Log, All);

class FUtilityAiEditorModule : public IModuleInterface
{
private:
	TSharedPtr<FExtender> mpBTToolbarExtender;
	TSharedPtr<const FExtensionBase> mpBTToolbarExtensionUtility;

	void AddBTEditorToolbarExtension(FToolBarBuilder& Builder);

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
