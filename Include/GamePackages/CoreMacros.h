#pragma once

#include <stdint.h>
#include <cstring>

#ifdef _WIN32
#ifdef GPKG_BUILD_LIBRARY
#define GPKG_API __declspec(dllexport)
#else
#define GPKG_API __declspec(dllimport)
#endif
#else
#define GPKG_API
#endif