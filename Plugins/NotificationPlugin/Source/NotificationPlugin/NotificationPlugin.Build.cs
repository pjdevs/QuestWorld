// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NotificationPlugin : ModuleRules
{
	public NotificationPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"SPUD"
			]
		);

		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG"
			]
		);
	}
}
