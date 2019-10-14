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
	IncludeDir["imgui"] = "%{prj.name}/lib/imgui"
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
			"%{prj.name}/lib/stb/stb_image.h",

			--"%{prj.name}/lib/imgui/imgui.h",
			--"%{prj.name}/lib/imgui/imgui_internal.h",
			--"%{prj.name}/lib/imgui/imgui_impl_glfw.h",
			--"%{prj.name}/lib/imgui/imstb_rectpack.h",
			--"%{prj.name}/lib/imgui/imstb_textedit.h",
			--"%{prj.name}/lib/imgui/imstb_truetype.h",
			--"%{prj.name}/lib/imgui/imgui_ogl3.h",
			--"%{prj.name}/lib/imgui/imconfig.h",
			--"%{prj.name}/lib/imgui/imgui_demo.cpp",
			--"%{prj.name}/lib/imgui/imgui_draw.cpp",
			--"%{prj.name}/lib/imgui/imgui_ogl3.cpp",
			--"%{prj.name}/lib/imgui/imgui_widgets.cpp"

		
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
			"%{prj.name}/lib/imgui"
		}
		
		links 
		{ 
            "GL",
            "GLEW",
			"glfw",
			"assimp"
			
		}