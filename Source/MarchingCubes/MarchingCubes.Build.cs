// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class MarchingCubes : ModuleRules
{

    //private string ModulePath
    //{
    //    get { return ModuleDirectory; }
    //}

    //private string ThirdPartyPath
    //{
    //    get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    //}

    public MarchingCubes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

        PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });

        //LoadLib(Target, "LibNoise", "LIB_NOISE");

    }

    //public bool LoadLib(ReadOnlyTargetRules Target, string libPath, string libName)
    //{
    //    bool isLibrarySupported = false;

    //    if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
    //    {
    //        isLibrarySupported = true;
    //        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
    //        string LibrariesPath = Path.Combine(ThirdPartyPath, libPath, "Libraries");

    //        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libPath + "." + PlatformString + ".lib"));
    //        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, libPath, "Includes"));
    //    }
    //    PublicDefinitions.Add(string.Format("WITH_" + libName + "_BINDING={0}", isLibrarySupported ? 1 : 0));
    //    return isLibrarySupported;
    //}
}
