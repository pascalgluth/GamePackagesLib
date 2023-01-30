#pragma once

#ifdef GPKG_BUILD_LIBRARY
#define GPKG_API __declspec(dllexport)
#else
#define GPKG_API __declspec(dllimport)
#endif