workspace "gmcl_rtmidi"
	startproject   "gmcl_rtmidi"
	includedirs    { "include" }
	platforms      { "x86", "x86_64" }
	configurations { "Release", "Debug" }

	filter { "platforms:x86" }
		libdirs      { "lib32" }
		architecture "x86"
	filter { "platforms:x86_64" }
		libdirs      { "lib64" }
		architecture "x86_64"

	filter { "configurations:Debug" }
		defines  { "DEBUG", "_DEBUG" }
		symbols  "On"
		runtime  "Debug"
		optimize "Debug"
	filter { "configurations:Release" }
		defines  { "NDEBUG", "_NDEBUG" }
		symbols  "Off"
		runtime  "Release"
		optimize "Speed"

	project "gmcl_rtmidi"
		files           { "src/gmcl_rtmidi.cpp" }
		links           { "rtmidi" }
		kind            "SharedLib"
		language        "C++"
		targetname      "rtmidi"
		targetprefix    "gmcl_"
		targetextension ".dll"

		filter { "system:macosx" }
			targetsuffix "_osx"
		filter { "system:windows", "platforms:x86" }
			targetsuffix "_win32"
		filter { "system:windows", "platforms:x86_64" }
			targetsuffix "_win64"
		filter { "system:linux", "platforms:x86" }
			targetsuffix "_linux"
		filter { "system:linux", "platforms:x86_64" }
			targetsuffix "_linux64"