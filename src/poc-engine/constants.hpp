#pragma once

namespace poc {

	inline constexpr char* engine_name = "PocEngine";
	inline constexpr int engine_version_major = 0;
	inline constexpr int engine_version_minor = 1;
	inline constexpr int engine_version_patch = 0;

#ifdef NDEBUG 
	inline constexpr bool isDebug = false;
#else
	inline constexpr bool isDebug = true;
#endif

}