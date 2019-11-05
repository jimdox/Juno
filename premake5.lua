workspace "BlackBox"
	architecture "x64"
	startproject "bx_engine"
	
	configurations
	{
		"Debug",
		"Release"
	
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	IncludeDir = {}
	IncludeDir["imgui"] = "%{prj.name}/lib/imgui"
	IncludeDir["spdlog"] = "%{prj.name}/lib/spdlog"
	IncludeDir["stb"] = "%{prj.name}/lib/stb"
	IncludeDir["jpl"] = "%{prj.name}/lib/jpl"
	
	
	project "bx_engine"
		location "bx_engine"
		kind "ConsoleApp"
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
			--"Imgui"
		}

		filter "configurations:Debug"
			defines { "BX_MODE_DEBUG" }
			symbols "on"

		filter "configurations:Release"
			optimize "on"	
		
