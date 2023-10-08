cmake_minimum_required(VERSION 3.15)
project(gamepackages CXX)

set(CMAKE_CXX_STANDARD 17)
if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

file(GLOB_RECURSE SOURCES Source/*.cpp)

add_definitions(-DGPKG_BUILD_LIBRARY)

add_library(gamepackages STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC Include)

find_package(ZLIB REQUIRED)
find_package(cryptopp REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ZLIB::ZLIB
        cryptopp::cryptopp
    )

install(TARGETS gamepackages)
install(DIRECTORY Include/GamePackages DESTINATION ${CMAKE_INSTALL_PREFIX}/include)