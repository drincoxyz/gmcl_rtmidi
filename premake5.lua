local api_allowed = {
	{ "winmm", "Windows Multimedia" },
	{ "jack",  "JACK Audio Connection Kit" },
	{ "alsa",  "Advanced Linux Sound Architecture" },
	{ "core",  "Apple Core Audio" },
}

local api_default = {
	windows = "winmm",
	linux   = "alsa",
	macosx  = "core",
}

newoption {
	trigger     = "api",
	value       = "API",
	allowed     = api_allowed,
	default     = api_default[os.target()],
	description = "Choose an API for RtMidi",
}

workspace "gmcl_rtmidi"
	startproject   "gmcl_rtmidi"
	cppdialect     "C++11"
	language       "C++"
	pic            "On"
	includedirs    { "include" }
	platforms      { "x86", "x86_64" }
	configurations { "Debug", "Release" }

	filter { "platforms:x86" }
		architecture "x86"
	filter { "platforms:x86_64" }
		architecture "x86_64"

	filter { "configurations:Debug" }
		runtime  "Debug"
		optimize "Debug"
		symbols  "On"
		defines  { "DEBUG" }
	filter { "configurations:Release" }
		runtime  "Release"
		optimize "Speed"
		symbols  "Off"
		defines  { "NDEBUG" }

	project "RtMidi"
		kind  "StaticLib"
		files { "src/RtMidi.cpp" }

		filter { "configurations:Debug" }
			defines { "__RTMIDI_DEBUG__" }

		filter { "options:api=winmm" }
			defines { "__WINDOWS_MM__" }
			links   { "winmm" }
		filter "options:api=jack"
			defines { "__UNIX_JACK__" }
			links   { "jack" }
		filter { "options:api=alsa" }
			defines { "__LINUX_ALSA__" }
			links   { "asound", "pthread" }
		filter { "options:api=core" }
			defines { "__MACOSX_CORE__" }
			links   { "CoreMIDI.framework", "CoreAudio.framework", "CoreFoundation.framework" }

	project "gmcl_rtmidi"
		kind            "SharedLib"
		files           { "src/gmcl_rtmidi.cpp" }
		links           { "RtMidi" }
		targetprefix    "gmcl_"
		targetname      "rtmidi"
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