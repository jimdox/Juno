workspace "Juno"
	architecture "x64"
	startproject "Sandbox"
	
	configurations {
		"Debug",
		"Release"
	
	}

	flags {
		--"MultiProcessorCompile"

	}
	
	outputdir = "%{cfg.buildcfg}"
	
IncludePaths = {}
IncludePaths["imgui"]  = "Juno/Vendor/imgui"
IncludePaths["spdlog"] = "Juno/Vendor/spdlog"
IncludePaths["stb"]    = "Juno/Vendor/stb"
IncludePaths["jpl"]    = "Juno/Vendor/jpl"
IncludePaths["glfw"]   = "Juno/Vendor/glfw"
IncludePaths["glew"]   = "Juno/Vendor/glew"
IncludePaths["glm"]    = "Juno/Vendor/glm"
	
project "Juno"
	location "Juno"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
    includedirs {

        "%{prj.name}/Src",
        "%{prj.name}/Include",
        "%{prj.name}/Vendor/stb",
        "%{prj.name}/Vendor/spdlog/include",
        "%{prj.name}/Vendor/jpl",
        "%{prj.name}/Vendor/imgui",
        "%{prj.name}/Vendor/glm",
        "%{prj.name}/",
        "%{prj.name}/Vendor/glew/include/",
        "%{prj.name}/Vendor/glew/lib"
    }
	
	targetdir ("./Bin/%{cfg.buildcfg}/")
	objdir ("./Bin/Int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "%{prj.name}/Include/jnpch.h"
	pchsource "%{prj.name}/Include/jnpch.cpp"
    


	files {
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.hpp",
        "%{prj.name}/Src/**.cpp",
        "%{prj.name}/Include/*.cpp",
        "%{prj.name}/Include/*.h",

		"%{prj.name}/Vendor/stb/stb_image.h",
		"%{prj.name}/Vendor/imgui/*.h",
		"%{prj.name}/Vendor/imgui/*.cpp"
	}
	
	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	links { 
--
	}

	filter "configurations:Debug"
		defines { "JN_BUILD_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"	

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	targetdir ("Bin/%{cfg.buildcfg}/")
	objdir ("Bin/Int/%{cfg.buildcfg}/")

	pchheader "Juno/Include/jnpch.h"
	pchsource "Juno/Include/jnpch.cpp"

	includedirs {
        "Juno",
        "Juno/Include",
        "Juno/Src",

        "Juno/Vendor/glm",
		"Juno/Vendor/glew/include",
		"Juno/Vendor/glew/lib",
		"Juno/Vendor/stb",
		"Juno/Vendor/jpl",
		"Juno/Vendor/imgui",
		"Juno/Vendor/spdlog/include"
	}

	files {
		"Sandbox/**.h",
		"Sandbox/**.cpp",
		"Juno/Include/jnpch.h",
		"Juno/Include/jnpch.cpp"
	}

	links {
		"Juno",
		"GL",
		"GLEW",
		"glfw"
	}

	defines {
		"STB_IMAGE_IMPLEMENTATION"
	}