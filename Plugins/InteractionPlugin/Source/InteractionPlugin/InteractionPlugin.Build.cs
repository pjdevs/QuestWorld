// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class InteractionPlugin : ModuleRules
{
	public InteractionPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"UMG",
				"Slate",
				"SlateCore",
				"EnhancedInput",
				"GameplayTags"
			]
		);
		
		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine"
			]
		);

		// Look for SPUD and include it if found
		string PluginsPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../.."));
		bool bShouldUseSpud = Directory.Exists(Path.Combine(PluginsPath, "SPUD"));

		if (bShouldUseSpud)
		{
			PublicDefinitions.Add("WITH_SPUD=1");
			PublicDependencyModuleNames.AddRange(
			[
					"SPUD"
				]
			);
		}
		else
		{
			PublicDefinitions.Add("WITH_SPUD=0");
		}
	}
}
