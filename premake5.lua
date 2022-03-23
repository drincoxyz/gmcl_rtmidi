workspace "gmod-midi"
	language      "C++"
	cppdialect    "C++latest"
	startproject  "midi"
	floatingpoint "Fast"

	includedirs    { "include" }
	platforms      { "x86", "x86_64" }
	configurations { "Debug", "Release" }

	filter { "configurations:Debug" }
		defines  { "DEBUG" }
		symbols  "On"
		optimize "Debug"
	filter { "configurations:Release" }
		defines  { "NDEBUG" }
		symbols  "Off"
		optimize "Speed"

	filter { "platforms:x86" }
		architecture "x86"
	filter { "platforms:x86_64" }
		architecture "x86_64"

	project "RtMidi"
		kind  "StaticLib"
		files { "src/RtMidi.cpp" }

		filter { "configurations:Debug" }
			defines { "__RTMIDI_DEBUG__" }

		filter { "system:windows" }
			links   { "winmm.lib" }
			defines { "__WINDOWS_MM__" }
		filter { "system:linux" }
			links   { "asound", "pthread" }
			defines { "__LINUX_ALSA__" }
		filter { "system:macosx" }
			links   { "CoreMIDI.framework", "CoreAudio.framework", "CoreFoundation.framework" }
			defines { "__MACOSX_CORE__" }

	project "gmod-midi"
		kind  "SharedLib"
		links { "RtMidi" }
		files { "src/gmod-midi.cpp" }
		
		targetname      "midi"
		targetprefix    "gmcl_"
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
			targetsuffix "_macosx"