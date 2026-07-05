using System.IO;

namespace UnrealBuildTool.Rules
{
	public class QuarkPhysics : ModuleRules
	{
		public QuarkPhysics(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			PrivatePCHHeaderFile = "Private/QuarkPhysicsPCH.h";

			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/QuarkPhysics"));

			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/QuarkPhysics"));

			PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"ProceduralMeshComponent"
			});

			PrivateDependencyModuleNames.AddRange(new string[] {
				"RenderCore",
				"RHI"
			});

			bWarningsAsErrors = false;

			// Suppress warnings for ported third-party physics library code
			CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Off;       // C4458, C4456, C4459
			CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Off;       // C4244, C4838
			CppCompileWarningSettings.ShortenSizeTToIntWarningLevel = WarningLevel.Off;    // C4267
			CppCompileWarningSettings.UnusedParameterWarningLevel = WarningLevel.Off;      // C4100
			CppCompileWarningSettings.DeprecationWarningLevel = WarningLevel.Off;          // C4996
			CppCompileWarningSettings.UnreachableCodeWarningLevel = WarningLevel.Off;      // C4702
			CppCompileWarningSettings.UninitializedWarningLevel = WarningLevel.Off;        // C4701
			CppCompileWarningSettings.SignCompareWarningLevel = WarningLevel.Off;          // C4018, C4389
			CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;  // C4668

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				PrivateDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
			}
		}
	}
}
