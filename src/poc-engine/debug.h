#pragma once

#ifdef NDEBUG 

constexpr bool isDebug = false;

#define LOG

#else

constexpr bool isDebug = true;

#include <iostream>
#define LOG(x) std::cerr << x << std::endl;

#endif