#pragma once

#include <vector>

namespace gpkg
{
    class Encryption
    {
    public:
        static std::vector<uint8_t> EncryptBytes(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv);
        static std::vector<uint8_t> DecryptBytes(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv);
        static void GenerateRSA256(std::vector<uint8_t> *outKey, std::vector<uint8_t> *outIV);
    };
}
