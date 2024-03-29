#include "GamePackages/Packager.h"

#include <filesystem>
#include <fstream>
#include <zlib.h>
#include <iostream>

#include "GamePackages/StreamedPackage.h"
#include "GamePackages/Encryption.h"

namespace gpkg
{

    void Packager::CreateStreamedPackage(const std::string &fullPath, const std::string &outputPath,
                                         const std::string &packageName, bool useCompression, bool encrypted,
                                         const std::vector<uint8_t> &key, const std::vector<uint8_t> &iv,
                                         std::function<void(int)> fileCountChangedCallback,
                                         std::function<void()> fileDoneCallback,
                                         std::function<void()> completedCallback)
    {
        std::vector<uint8_t> dataBytes;
        std::vector<FileEntry> lookupTable;

        if (!std::filesystem::exists(fullPath) || !std::filesystem::is_directory(fullPath))
            return;

        int count = 0;

        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(fullPath))
        {
            if (!dirEntry.is_directory())
            {
                count++;
            }
        }

        if (fileCountChangedCallback)
        {
            fileCountChangedCallback(count);
        }

        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(fullPath))
        {
            if (!dirEntry.is_directory())
            {
                std::vector<uint8_t> fileBytes = ReadFileBytes(dirEntry.path().string());
                if (fileBytes.size() <= 0) continue;

                if (useCompression)
                {
                    std::vector<uint8_t> compressedData(compressBound(fileBytes.size()));
                    uLongf compressedDataSize = compressedData.size();
                    int result = compress(&compressedData[0], &compressedDataSize, fileBytes.data(), fileBytes.size());
                    if (result != Z_OK)
                        throw std::runtime_error("Failed to compress data");
                    compressedData.resize(compressedDataSize);

                    fileBytes.clear();
                    fileBytes.insert(fileBytes.end(), compressedData.begin(), compressedData.end());
                }

                if (encrypted)
                {
                    fileBytes = Encryption::EncryptBytes(fileBytes, key, iv);
                }

                std::string relative = std::filesystem::relative(dirEntry.path(), fullPath).string();
                uint64_t offset = dataBytes.size();
                uint64_t size = fileBytes.size();

                std::replace(relative.begin(), relative.end(), '\\', '/');

                dataBytes.insert(dataBytes.end(), fileBytes.begin(), fileBytes.end());
                lookupTable.emplace(lookupTable.end(), FileEntry(relative.c_str(), offset, size));
            
                if (fileDoneCallback) fileDoneCallback();
            }
        }

        std::vector<uint8_t> lookupTableBytes(lookupTable.size() * sizeof(FileEntry), 0);
        memcpy(lookupTableBytes.data(), lookupTable.data(), lookupTable.size() * sizeof(FileEntry));

        if (encrypted)
        {
            lookupTableBytes = Encryption::EncryptBytes(lookupTableBytes, key, iv);
        }

        std::string pkgFile = packageName + ".gpkg";

        if (!std::filesystem::exists(outputPath))
            std::filesystem::create_directory(outputPath);

        std::filesystem::path pkgPath = std::filesystem::path(outputPath) / std::filesystem::path(pkgFile);
        if (std::filesystem::exists(pkgPath) && !std::filesystem::is_directory(pkgPath))
            std::filesystem::remove(pkgPath);

        std::ofstream outFile;
        outFile.open(pkgPath, std::ios::binary | std::ios::out);

        char fileDef[5] = { 0x5f, 0x47, 0x50, 0x4b, 0x47 };
        outFile.write(fileDef, 5);

        uint64_t lookupTableSize = lookupTableBytes.size();

        outFile.write((char*)&lookupTableSize, sizeof(uint64_t));
        outFile.write((char*)lookupTableBytes.data(), lookupTableBytes.size());
        outFile.write((char*)dataBytes.data(), dataBytes.size());

        outFile.close();

        if (completedCallback) completedCallback();
    }

    std::vector<uint8_t> Packager::ReadFileBytes(const std::string &path)
    {
        std::vector<uint8_t> data;

        std::ifstream inFile;
        inFile.open(path, std::ios::binary | std::ios::in);

        inFile.seekg(0, std::ios::end);
        size_t length = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        char* pData = (char*)calloc(length, sizeof(char));
        inFile.read(pData, length);

        data.insert(data.end(), pData, pData+length);

        delete [] pData;

        return std::move(data);
    }

    void Packager::WriteFileBytes(const std::string &path, const std::vector<uint8_t> &bytes)
    {
        if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
            std::filesystem::remove(path);

        std::ofstream outFile;
        outFile.open(path, std::ios::binary | std::ios::out);
        outFile.write((char*)bytes.data(), bytes.size());
        outFile.close();
    }
}
