// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TrainGame : ModuleRules
{
	public TrainGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate",
			"Interaction"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TrainGame",
			"TrainGame/Public",
			"TrainGame/Public/GameFramework",
			"TrainGame/Public/Train",
			"TrainGame/Public/Train/Components",
			"TrainGame/Public/World",
			"TrainGame/Public/Variant_Strategy",
			"TrainGame/Public/Variant_Strategy/UI",
			"TrainGame/Public/Variant_TwinStick",
			"TrainGame/Public/Variant_TwinStick/AI",
			"TrainGame/Public/Variant_TwinStick/Gameplay",
			"TrainGame/Public/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
