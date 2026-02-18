using UnrealBuildTool;

public class AsherUtils : ModuleRules
{
	public AsherUtils(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"AssetTools",
				"AssetRegistry",
				"ContentBrowser",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"ToolMenus",
				"Projects",
				"FortniteGame"
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputCore",
				"LevelEditor",
				"WorkspaceMenuStructure",
				"PropertyEditor"
			}
		);
	}
}
