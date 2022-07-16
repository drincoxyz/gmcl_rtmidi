newoption {
	trigger = "jack",
	description = "Use JACK Audio Connection Kit (libjack)"
}

workspace "gmcl_rtmidi"
	startproject "gmcl_rtmidi"
	includedirs { "include" }
	platforms { "x86", "x86_64" }
	configurations { "Debug", "Release" }

	filter "platforms:x86"
		architecture "x86"
	filter "platforms:x86_64"
		architecture "x86_64"

	filter "system:windows"
		defines { "WIN32", "_WIN32" }
	filter "system:linux"
		defines { "__linux__" }
	filter "system:macosx"
		defines { "__APPLE__" }

	filter { "platforms:x86_64", "system:windows" }
		defines { "_M_X64" }
	filter { "platforms:x86_64", "system:linux" }
		defines { "__x86_64__" }
	filter { "platforms:x86_64", "system:macosx" }
		defines { "__x86_64__" }

	filter "configurations:Debug"
		runtime "Debug"
		optimize "Debug"
		symbols "On"
		defines { "DEBUG", "_DEBUG" }
	filter "configurations:Release"
		runtime "Release"
		optimize "Speed"
		symbols "Off"
		defines { "NDEBUG", "_NDEBUG" }

project "rtmidi"
	language "C++"
	kind "StaticLib"
	pic "On"
	files { "src/RtMidi.cpp" }

	filter "configurations:Debug"
		defines { "__RTMIDI_DEBUG__" }
	filter "system:windows"
		defines { "RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES" }

	filter "options:jack"
		defines { "__UNIX_JACK__" }
		links { "jack" }
	filter "system:windows"
		defines { "__WINDOWS_MM__" }
		links { "winmm" }
	filter "system:linux"
		defines { "__LINUX_ALSA__" }
		links { "asound", "pthread" }
	filter "system:macosx"
		defines { "__MACOSX_CORE__" }
		links { "CoreMIDI.framework", "CoreAudio.framework", "CoreFoundation.framework" }

project "gmcl_rtmidi"
	language "C++"
	kind "SharedLib"
	pic "On"
	files { "src/gmcl_rtmidi.cpp" }
	links { "rtmidi" }
	targetprefix "gmcl_"
	targetname "rtmidi"
	targetextension ".dll"

	filter { "system:windows", "platforms:x86" }
		targetsuffix "_win32"
	filter { "system:windows", "platforms:x86_64" }
		targetsuffix "_win64"
	filter { "system:linux", "platforms:x86" }
		targetsuffix "_linux"
	filter { "system:linux", "platforms:x86_64" }
		targetsuffix "_linux64"
	filter { "system:macosx" }
		targetsuffix "_osx"
