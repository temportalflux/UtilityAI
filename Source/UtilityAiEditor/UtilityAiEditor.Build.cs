// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UtilityAiEditor : ModuleRules
{
    public UtilityAiEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
				"UtilityAi/Private",
				"UtilityAiEditor/Private",
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				// ... add other public dependencies that you statically link with here ...
				"CoreUObject",
                "Engine",

                "AIModule",
                "GameplayTasks",

                "Slate",
                "SlateCore",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
				// ... add private dependencies that you statically link with here ...
                "Projects",
                "Core",
                "CoreUObject",
                "LevelEditor",
                "ApplicationCore",
                "Engine",
                "RenderCore",
                "InputCore",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "UnrealEd",
                "AudioEditor",
                "MessageLog",
                "GraphEditor",
                "Kismet",
                "KismetWidgets",
                "PropertyEditor",
                "AnimGraph",
                "BlueprintGraph",
                "AIGraph",
                "AIModule",
                "ClassViewer",
                "BehaviorTreeEditor",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
