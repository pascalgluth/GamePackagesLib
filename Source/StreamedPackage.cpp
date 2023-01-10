#include "GamePackages/StreamedPackage.h"

#include <fstream>

#include "GamePackages/Encryption.h"

namespace gpkg
{
    StreamedPackage::StreamedPackage(const std::string &fullPath, bool encrypted, const std::vector<uint8_t> &key,
                                     const std::vector<uint8_t> &iv)
    {
        m_fullPath = fullPath;
        m_encrypted = encrypted;
        m_key = key;
        m_iv = iv;

        std::ifstream inFile;

        inFile.open(fullPath, std::ios::binary | std::ios::in);
        if (!inFile.is_open())
            throw std::runtime_error("Failed to open package file: " + fullPath);

        char fileDef[5];
        inFile.read(fileDef, 5);

        if (!(fileDef[0] == 0x5f &&
              fileDef[1] == 0x47 &&
              fileDef[2] == 0x50 &&
              fileDef[3] == 0x4b &&
              fileDef[4] == 0x47))
        {
            std::runtime_error("Invalid package file: " + fullPath);
        }

        char readBuffer[sizeof(uint64_t)];
        inFile.seekg(5);
        inFile.read(readBuffer, sizeof(uint64_t));
        uint64_t* lookupTableSize = reinterpret_cast<uint64_t*>(readBuffer);

        m_dataOffset = 5 + sizeof(uint64_t) + *lookupTableSize;

        std::vector<char> readData(*lookupTableSize, 0);
        inFile.read(readData.data(), *lookupTableSize);

        std::vector<uint8_t> lookupTableBytes;
        lookupTableBytes.insert(lookupTableBytes.end(), readData.begin(), readData.end());

        if (m_encrypted)
        {
            lookupTableBytes = Encryption::DecryptBytes(lookupTableBytes, m_key, m_iv);
        }

        m_lookupTable.resize(lookupTableBytes.size() / sizeof(FileEntry));
        memcpy(m_lookupTable.data(), lookupTableBytes.data(), lookupTableBytes.size());
    }

    std::vector<uint8_t> StreamedPackage::ReadFileBytes(const std::string &relativePath)
    {
        std::vector<uint8_t> bytes;

        for (int i = 0; i < m_lookupTable.size(); ++i)
        {
            if (std::string(m_lookupTable[i].relativePath) == relativePath)
            {
                std::vector<char> bytesRead(m_lookupTable[i].size, 0);

                std::ifstream inFile;
                inFile.open(m_fullPath, std::ios::binary | std::ios::in);
                inFile.seekg(m_dataOffset + m_lookupTable[i].offset);
                inFile.read(bytesRead.data(), m_lookupTable[i].size);
                inFile.close();

                bytes.insert(bytes.end(), bytesRead.begin(), bytesRead.end());

                if (m_encrypted)
                {
                    bytes = Encryption::DecryptBytes(bytes, m_key, m_iv);
                }

                break;
            }
        }

        return std::move(bytes);
    }

    std::string StreamedPackage::ReadFileText(const std::string &relativePath)
    {
        std::vector<uint8_t> bytes = ReadFileBytes(relativePath);

        char buffer[bytes.size()];
        //memcpy(buffer, bytes.data(), bytes.size());
        strcpy(buffer, reinterpret_cast<char*>(bytes.data()));

        return std::string(buffer);
    }
}