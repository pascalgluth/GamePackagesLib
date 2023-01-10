#pragma once

#include <vector>
#include <string>

namespace gpkg
{
    class IDataPackage
    {
    public:
        virtual std::vector<uint8_t> ReadFileBytes(const std::string& relativePath) = 0;
        virtual std::string ReadFileText(const std::string& relativePath) = 0;

    };
}
