using UnrealBuildTool;

public class MazeGenModule: ModuleRules
{
    public MazeGenModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
    }
}
