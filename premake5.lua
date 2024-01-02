workspace "SFML Project"
    configurations { "debug", "release" }
    language "C++"
    cppdialect "C++17"
    startproject "MyProject"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    project "Engine"
        kind "StaticLib"
        location "build/%{prj.name}"
        targetdir "bin/%{cfg.buildcfg}"
        objdir "bin/int/%{cfg.buildcfg}"

        files 
        { 
            "engine/src/**.cpp",
            "engine/src/**.hpp",
            
            -- imgui

            "engine/vendor/imgui/*.cpp",
            "engine/vendor/imgui/*.h",
            "engine/vendor/imgui-sfml/*.cpp",
            "engine/vendor/imgui-sfml/*.h",

            "engine/vendor/json/json.hpp"
        }

        includedirs 
        { 
            "engine/src",
            "engine/vendor/imgui",
            "engine/vendor/imgui-sfml",
            "engine/vendor/json"
        }

        libdirs 
        { 
            "sfml/lib" 
        }

        links
        {
            "sfml-graphics", 
            "sfml-window", 
            "sfml-system",
            "sfml-audio",
            "OpenGL.framework"
        }

    project "MyGame"
        targetextension ""
        kind "WindowedApp"
        location "build/%{prj.name}"
        targetdir "bin/%{cfg.buildcfg}"
        objdir "bin/int/%{cfg.buildcfg}"

        files 
        { 
            "game/src/**.cpp",
            "game/src/**.hpp" 
        }
        
        links 
        { 
            "engine",
            "sfml-graphics", 
            "sfml-window", 
            "sfml-system",
            "sfml-audio",
            "OpenGL.framework"
        }
        
        includedirs 
        { 
            "engine/src",
            "engine/vendor/imgui",
            "engine/vendor/imgui-sfml",
            "engine/vendor/json"
        }