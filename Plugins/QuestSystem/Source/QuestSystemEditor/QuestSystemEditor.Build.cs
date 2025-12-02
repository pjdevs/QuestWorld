using UnrealBuildTool;

public class QuestSystemEditor : ModuleRules
{
    public QuestSystemEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "QuestSystem",
                "GameplayTags",
                "UnrealEd",
                "AssetDefinition"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "FlowEditor",
                "AssetTools",
                "InputCore"
            ]
        );
    }
}