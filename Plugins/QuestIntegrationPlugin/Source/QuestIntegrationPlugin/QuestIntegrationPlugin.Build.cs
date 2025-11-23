// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuestIntegrationPlugin : ModuleRules
{
	public QuestIntegrationPlugin(ReadOnlyTargetRules Target) : base(Target)
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
			}
		);
		
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DialogPlugin",
				"InventoryPlugin",
				"GameplayAbilities",
				"GameplayAbilitySystemPlugin",
				"SPUD",
				"Flow"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"QuestSystem",
				"NotificationPlugin",
				"InteractionPlugin",
				"DialogPlugin",
				"InventoryPlugin",
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
