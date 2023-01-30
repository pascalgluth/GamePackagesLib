#pragma once

#include <string>
#include <vector>

#include "CoreMacros.h"

namespace gpkg
{
    class GPKG_API Packager
    {
    public:
        static void CreateStreamedPackage(const std::string& fullPath, const std::string& outputPath, const std::string& packageName, bool useCompression = false, bool encrypted = false, const std::vector<uint8_t>& key = {}, const std::vector<uint8_t>& iv = {});
        static std::vector<uint8_t> ReadFileBytes(const std::string& path);
        static void WriteFileBytes(const std::string& path, const std::vector<uint8_t>& bytes);

    };
}