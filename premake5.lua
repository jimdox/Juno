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
		--"MultiProcessorCompile"

	}
	
	outputdir = "%{cfg.buildcfg}"
	
IncludeDir = {}
IncludeDir["imgui"] = "%{prj.name}/vendor/imgui"
IncludeDir["spdlog"] = "%{prj.name}/vendor/spdlog"
IncludeDir["stb"] = "%{prj.name}/vendor/stb"
IncludeDir["jpl"] = "%{prj.name}/vendor/jpl"
IncludeDir["glfw"] = "%{prj.name}/vendor/glfw"
IncludeDir["glew"] = "%{prj.name}/vendor/glew"
IncludeDir["glm"] = "%{prj.name}/vendor/glm"
	
project "engine"
	location "engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	includedirs {
		"%{prj.name}/vendor/imgui", 
		"%{prj.name}/vendor/spdlog",
		"%{prj.name}/vendor/stb",
		"%{prj.name}/vendor/jpl",
		"%{prj.name}/vendor/glm"
	}
	
	targetdir ("./bin/%{cfg.buildcfg}/")
	objdir ("./bin/int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "%{prj.name}/pch.h"
	pchsource "%{prj.name}/pch.cpp"
		
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
		"%{prj.name}/vendor/stb",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/jpl",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/",
		"%{prj.name}/vendor/glew/include/",
		"%{prj.name}/vendor/glew/lib"
	}
	
	files
	{
		"%{prj.name}/include/core/**.h",
		"%{prj.name}/include/render/**.h",
		"%{prj.name}/include/entity/**.h",
		"%{prj.name}/include/events/**.h",
		"%{prj.name}/include/gui/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb/stb_image.h",
		"%{prj.name}/vendor/jpl/*.hpp",
		"%{prj.name}/vendor/imgui/*.h",
		"%{prj.name}/vendor/imgui/*.cpp",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	links 
	{ 
--
	}

	filter "configurations:Debug"
		defines { "JN_BUILD_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"	

project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	targetdir ("bin/%{cfg.buildcfg}/")
	objdir ("bin/int/%{cfg.buildcfg}/")

	pchheader "engine/pch.h"
	pchsource "engine/pch.cpp"

	includedirs
	{
		"engine/include",
		"engine/vendor/glm",
		
		"engine/vendor/glew/include/",
		"engine/vendor/glew/lib",

		"engine/vendor/stb",
		"engine/vendor/jpl",
		"engine/vendor/imgui",
		"engine/vendor/spdlog/include",
		"engine"
	}

	files
	{
		"sandbox/**.h",
		"sandbox/**.cpp",
		"engine/pch.h",
		"engine/pch.cpp"
	}

	links
	{
		"engine",
		"GL",
		"GLEW",
		"glfw"
	}

	defines
	{
		"STB_IMAGE_IMPLEMENTATION"
	}