#pragma once

#include "IDataPackage.h"

namespace gpkg
{
    struct FileEntry
    {
        char relativePath[128]; // 128 bytes
        uint64_t offset; // 8 bytes
        uint64_t size; // 8 bytes

        FileEntry()
        {
            for (int i = 0; i < sizeof(relativePath); ++i)
                relativePath[i] = '\0';
            offset = 0;
            size = 0;
        }

        FileEntry(const char* _relativePath, uint64_t _offset, uint64_t _size)
            : FileEntry()
        {
            strcpy(relativePath, _relativePath);
            offset = _offset;
            size = _size;
        }
    };

    class StreamedPackage : public IDataPackage
    {
    public:
        StreamedPackage(const std::string& fullPath, bool useCompression = false, bool encrypted = false, const std::vector<uint8_t>& key = {}, const std::vector<uint8_t>& iv = {});

        std::vector<uint8_t> ReadFileBytes(const std::string &relativePath) override;
        std::string ReadFileText(const std::string &relativePath) override;

    private:
        std::string m_fullPath;
        bool m_useCompression;
        bool m_encrypted;
        std::vector<uint8_t> m_key;
        std::vector<uint8_t> m_iv;

        std::vector<FileEntry> m_lookupTable;
        uint64_t m_dataOffset;

    };
}