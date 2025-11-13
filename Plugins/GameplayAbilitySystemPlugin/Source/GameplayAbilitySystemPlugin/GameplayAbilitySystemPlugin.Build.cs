// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameplayAbilitySystemPlugin : ModuleRules
{
	public GameplayAbilitySystemPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"UMG",
				"SPUD"
			]
		);

		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			]
		);
	}
}
