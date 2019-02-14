// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ToolbarStyle.h"

class FToolbarCommands : public TCommands<FToolbarCommands>
{
public:

	FToolbarCommands()
		: TCommands<FToolbarCommands>(TEXT("UtilityAiEditor"), NSLOCTEXT("Contexts", "UtilityAiEditor", "Utility AI Plugin"), NAME_None, FToolbarStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > Action_UtilityTree_OpenWizard;
};
