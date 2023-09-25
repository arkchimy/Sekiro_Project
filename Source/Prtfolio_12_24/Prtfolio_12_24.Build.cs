using UnrealBuildTool;

public class Prtfolio_12_24 : ModuleRules
{
	public Prtfolio_12_24(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "Niagara", "AIModule" , "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		
		PublicIncludePaths.Add(ModuleDirectory);
	}
}
