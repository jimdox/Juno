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
	
project "juno_engine"
	location "juno_engine"
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
		"juno_engine/vendor/glew/include/",
		"juno_engine/vendor/glew/lib"
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

	pchheader "juno_engine/pch.h"
	pchsource "juno_engine/pch.cpp"

	includedirs
	{
		"juno_engine/include",
		"juno_engine/vendor/glm",
		
		"juno_engine/vendor/glew/include/",
		"juno_engine/vendor/glew/lib",

		"juno_engine/vendor/stb",
		"juno_engine/vendor/jpl",
		"juno_engine/vendor/imgui",
		"juno_engine/vendor/spdlog/include",
		"juno_engine"
	}

	files
	{
		"sandbox/**.h",
		"sandbox/**.cpp",
		"juno_engine/pch.h",
		"juno_engine/pch.cpp"
	}

	links
	{
		"juno_engine",
		"GL",
		"GLEW",
		"glfw"
	}

	defines
	{
		"STB_IMAGE_IMPLEMENTATION"
	}