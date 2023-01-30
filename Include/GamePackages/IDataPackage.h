#pragma once

#include <vector>
#include <string>

#include "CoreMacros.h"

namespace gpkg
{
    class GPKG_API IDataPackage
    {
    public:
        virtual std::vector<uint8_t> ReadFileBytes(const std::string& relativePath) = 0;
        virtual std::string ReadFileText(const std::string& relativePath) = 0;
        virtual int FileCount() = 0;
        virtual bool Contains(const std::string& file) = 0;

    };
}
