-- Thank you to this very kind person who posted a guide on linking GLAD + GLFW on github: https://github.com/HectorPeeters
project "GLAD"
kind "StaticLib"
language "C"
architecture "x86_64"

targetdir "../../../Bin/%{cfg.buildcfg}"
objdir "../../../Bin/Int/%{cfg.buildcfg}"

includedirs {"include/"}

files {
    "src/glad.c",
}

filter "system:linux"
    pic "On"

    systemversion "latest"
    staticruntime "On"

    defines { "_GLAD_X11" }

filter "system:windows"
    systemversion "latest"
    staticruntime "On"

    defines {
        "_GLAD_WIN32",
        "_CRT_SECURE_NO_WARNINGS"
    }

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"

    