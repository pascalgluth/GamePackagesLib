# GamePackagesLib

A library to create game packages for game assets.

## Adding library to CMake project

- Add the library directory to your CMake project with `add_subdirectory(...)`
- Link to the library with `target_link_libraries(${PROJECT_NAME} GamePackagesLib)`

## Building

- The library can be built as a static library

## Examples

### File Structure

- All files in the given input directory will be recursively added to the package
- E.g. With input "../testInput"
  - "../testInput/myImage.png" will be "myImage.png" in the Package
  - "../testInput/subFolder/text.txt" will be "subFolder/text.txt" in the Package

### Create and read streamed package without encryption

```cpp
#include "GamePackages/StreamedPackage.h"
#include "GamePackages/Packager.h"

int main()
{
    // Create the Package
    gpkg::Packager::CreateStreamedPackage("../testInput", "../testOutput", "TestPackage");
    
    // Read the Package later
    gpkg::StreamedPackage myPackage = { "../testOutput/TestPackage.gpkg" };
    
    return 0;
}
```

### Create and read streamed package with encryption

```cpp
#include "GamePackages/StreamedPackage.h"
#include "GamePackages/Packager.h"
#include "GamePackages/Encryption.h"

int main()
{
    // Generate encryption keys
    std::vector<uint8_t> key, iv;
    gpkg::Encryption::GenerateRSA256(&key, &iv);
    
    // Create the package
    gpkg::Packager::CreateStreamedPackage("../testInput", "../testOutput", "EncryptedTestPackage", true, key, iv);
    
    // Read the package later
    gpkg::StreamedPackage encryptedPackage = { "../testOutput/EncryptedTestPackage.gpkg", true, key, iv };
    
    return 0;
}
```

### Accessing files in streamed packages

```cpp
gpkg::StreamedPackage myPackage = {...};

// Read file as text
std::string fileText = myPackage.ReadFileText("myAwesomeText.txt");

// Read file as byte array
std::vector<uint8_t> myImage = myPackage.ReadFileBytes("myImage.png");
```