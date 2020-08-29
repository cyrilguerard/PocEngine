#pragma once

#ifdef NDEBUG 

constexpr bool isDebug = false;

#define LOG

#else

constexpr bool isDebug = true;

#endif