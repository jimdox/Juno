workspace "BlackBox"
	architecture "x64"
	startproject "Userspace"
	
	configurations
	{
		"Debug",
		"Release"
	
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	IncludeDir = {}
	IncludeDir["GLFW"] = "BlackBox_Engine/lib/glfw/include"
	IncludeDir["Glad"] = "BlackBox_Engine/lib/glew/include"
	IncludeDir["ImGui"] = "BlackBox_Engine/lib/imgui"
	IncludeDir["glm"] = "BlackBox_Engine/lib/glm"
	IncludeDir["spdlog"] = "BlackBox_Engine/lib/spdlog"
	IncludeDir["stb"] = "BlackBox_Engine/lib/stb"
	
	
	
	group "Dependencies"
		include "BlackBox/lib/GLFW"
		include "BlackBox/lib/Glad"
		include "BlackBox/lib/imgui"
		include "BlackBox/lib/spdlog"
		include "BlackBox/lib/stb"
		include "BlackBox/lib/assimp"
		


	project "BlackBox"
		location "BlackBox"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("bin/ .. outputdir .. "/%{prj.name}")
		objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
		
		pchheader "pch.h"
		
		pchsource "BlackBox/src/pch.cpp"
		
		
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl",
		
		}
		
		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}"
		}
		
		links 
		{ 
			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib"
		}
	
		
	
