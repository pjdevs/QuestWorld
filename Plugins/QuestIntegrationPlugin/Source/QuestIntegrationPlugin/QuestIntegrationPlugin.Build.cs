// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuestIntegrationPlugin : ModuleRules
{
	public QuestIntegrationPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"DialogPlugin",
				"InventoryPlugin",
				"GameplayAbilities",
				"GameplayAbilitySystemPlugin",
				"SPUD",
				"Flow",
				"QuestSystem"
			]
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"NotificationPlugin",
				"InteractionPlugin",
				"DialogPlugin",
				"InventoryPlugin"
			]
		);
	}
}
