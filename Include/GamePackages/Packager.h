#pragma once

#include <string>
#include <vector>
#include <functional>

#include "CoreMacros.h"

namespace gpkg
{
    namespace Packager
    {
        GPKG_API void CreateStreamedPackage(const std::string& fullPath, const std::string& outputPath, const std::string& packageName, bool useCompression = false, bool encrypted = false, const std::vector<uint8_t>& key = {}, const std::vector<uint8_t>& iv = {},
                                         std::function<void(int)> fileCountChangedCallback = {},
                                         std::function<void()> fileDoneCallback = {},
                                         std::function<void()> completedCallback = {});
        GPKG_API std::vector<uint8_t> ReadFileBytes(const std::string& path);
        GPKG_API void WriteFileBytes(const std::string& path, const std::vector<uint8_t>& bytes);

    };
}