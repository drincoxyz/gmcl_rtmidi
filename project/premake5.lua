workspace "midi"
	platforms      { "x86", "x86_64" }
	configurations { "Debug", "Release" }

project "midi"
	kind             "SharedLib"
	language         "C++"
	symbols          "On"
	staticruntime    "On"
	editandcontinue  "Off"
	vectorextensions "SSE"
	targetextension  ".dll"
	floatingpoint    "Fast"
	objdir           "../object"
	targetdir        "../build"
	flags            { "NoPCH" }
	defines          { "GMMODULE" }
	files            { "../include/**.h", "../source/**.cpp" }
	includedirs      { "../include" }

filter "platforms:x86"
	architecture "x86"

filter "platforms:x86_64"
	architecture "x86_64"

filter "system:windows"
	links   { "winmm.lib" }
	defines { "__WINDOWS_MM__" }

filter "system:linux"
	links   { "asound", "pthread" }
	defines { "__LINUX_ALSA__" }

filter "system:macosx"
	links      { "CoreMIDI.framework", "CoreAudio.framework", "CoreFoundation.framework" }
	defines    { "__MACOSX_CORE__" }
	targetname "gmcl_midi_osx"

filter { "system:windows", "platforms:x86" }
	targetname "gmcl_midi_win32"

filter { "system:linux", "platforms:x86" }
	targetname "gmcl_midi_linux"

filter { "system:windows", "platforms:x86_64" }
	targetname "gmcl_midi_win64"

filter { "system:linux", "platforms:x86_64" }
	targetname "gmcl_midi_linux64"

filter "configurations:Release"
	defines  { "NDEBUG" }
	optimize "Speed"

filter "configurations:Debug"
	defines  { "DEBUG", "RTMIDI_DEBUG", "__RTMIDI_DEBUG__" }
	optimize "Debug"