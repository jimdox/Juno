workspace "Juno"
	architecture "x64"
	startproject "sandbox"
	
	configurations
	{
		"Debug",
		"Release"
	
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
	outputdir = "%{cfg.buildcfg}"
	
IncludeDir = {}
IncludeDir["imgui"] = "%{prj.name}/lib/imgui"
IncludeDir["spdlog"] = "%{prj.name}/lib/spdlog"
IncludeDir["stb"] = "%{prj.name}/lib/stb"
IncludeDir["jpl"] = "%{prj.name}/lib/jpl"
	
	
	project "juno_engine"
		location "juno_engine"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("./bin/%{cfg.buildcfg}/")
		objdir ("./bin/int/" .. outputdir .. "/%{prj.name}")
		
		pchheader "%{prj.name}/pch.h"
		pchsource "%{prj.name}/pch.cpp"
		
		
		files
		{
			"%{prj.name}/include/core/**.h",
			"%{prj.name}/include/render/**.h",
			"%{prj.name}/include/entity/**.h",
			"%{prj.name}/include/events/**.h",
			"%{prj.name}/include/gui/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/lib/stb/stb_image.h",

			"%{prj.name}/lib/jpl/*.hpp",

			"%{prj.name}/lib/imgui/*.h",
			"%{prj.name}/lib/imgui/*.cpp",

			
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
			"%{prj.name}/lib/jpl",
			"%{prj.name}/lib/imgui",
			"%{prj.name}/"

		}
		
		links 
		{ 
            "GL",
            "GLEW",
			"glfw"
		}

		filter "configurations:Debug"
			defines { "JN_MODE_DEBUG" }
			symbols "on"

		filter "configurations:Release"
		optimize "on"	

project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	targetdir ("bin/%{cfg.buildcfg}/")
	objdir ("bin/int/%{cfg.buildcfg}/")

	pchheader "juno_engine/pch.h"
	pchsource "juno_engine/pch.cpp"

	files
	{
		"sandbox/**.h",
		"sandbox/**.cpp",
		"juno_engine/pch.h",
		"juno_engine/pch.cpp"
	}

	includedirs
	{
		"juno_engine/include",
		"juno_engine/lib/stb",
		"juno_engine/lib/jpl",
		"juno_engine/lib/imgui",
		"juno_engine/lib/spdlog/include",
		"juno_engine"
	}

	links
	{
		"juno_engine",
		"GL",
		"GLEW",
		"glfw"
	}


