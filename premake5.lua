workspace "BlackBox"
	architecture "x64"
	startproject "bxEngine"
	
	configurations
	{
		"Debug",
		"Release"
	
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	IncludeDir = {}
	--IncludeDir["ImGui"] = "%{prj.name}/lib/imgui"
	IncludeDir["spdlog"] = "%{prj.name}/lib/spdlog"
	IncludeDir["stb"] = "%{prj.name}/lib/stb"
	
	
	project "bxEngine"
		location "bxEngine"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("./bin/")
		objdir ("./bin/int/" .. outputdir .. "/%{prj.name}")
		
		pchheader "%{prj.name}/pch.h"
		pchsource "%{prj.name}/pch.cpp"
		
		
		files
		{
			"%{prj.name}/include/core/**.h",
			"%{prj.name}/include/render/**.h",
			"%{prj.name}/include/entity/**.h",
			"%{prj.name}/include/events/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/lib/stb/stb_image.h"
		
		}
		
		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/include",
			"%{prj.name}/lib/stb",
			"%{prj.name}/lib/spdlog/include",
			--"%{prj.name}/lib/imgui",
		}
		
		links 
		{ 
            "GL",
            "GLEW",
			"glfw",
			"assimp"
			
		}