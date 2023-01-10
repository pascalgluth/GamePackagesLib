cmake_minimum_required(VERSION 3.24)
project(GamePackagesLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES Source/*.cpp)

add_library(GamePackagesLib ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC Include)

add_subdirectory(Libs/zlib)
target_link_libraries(${PROJECT_NAME} zlibstatic)
target_include_directories(${PROJECT_NAME} PRIVATE Libs)

add_subdirectory(Libs/cryptopp-cmake)
target_link_libraries(${PROJECT_NAME} cryptopp)

#project(TestMain)
#add_executable(TestMain main.cpp)
#target_link_libraries(TestMain GamePackagesLib)