// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuestWorld : ModuleRules
{
	public QuestWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"InteractionPlugin",
			"QuestSystem"
		});
	}
}