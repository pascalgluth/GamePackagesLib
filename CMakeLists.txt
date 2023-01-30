cmake_minimum_required(VERSION 3.24)
project(GamePackagesLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")

file(GLOB_RECURSE SOURCES Source/*.cpp)

add_definitions(-DGPKG_BUILD_LIBRARY)

add_library(GamePackagesLib SHARED ${SOURCES})

add_subdirectory(Libs/zlib)
add_subdirectory(Libs/cryptopp-cmake)

target_include_directories(${PROJECT_NAME} PUBLIC Include)

target_link_libraries(${PROJECT_NAME} zlibstatic)
#add_dependencies(${PROJECT_NAME} zlibstatic)
target_include_directories(${PROJECT_NAME} PRIVATE Libs)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Libs/zlib)

target_link_libraries(${PROJECT_NAME} cryptopp)
#add_dependencies(${PROJECT_NAME} cryptopp)
#target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Libs/cryptopp-cmake/_deps/cryptopp)

#set(LIBNAME "libGamePackagerLib.a")

#add_custom_command(
#        OUTPUT ${LIBNAME}
#        COMMAND ar -rcT ${LIBNAME} $<TARGET_FILE:GamePackagesLib> $<TARGET_FILE:zlibstatic> $<TARGET_FILE:cryptopp>
#        DEPENDS GamePackagesLib zlibstatic cryptopp
#        COMMENT "Creating combined library..."
#)

#add_custom_target(GamePackagesLibCombined DEPENDS ${LIBNAME})

#project(TestMain)
#add_executable(TestMain main.cpp)
#target_link_libraries(TestMain GamePackagesLib)