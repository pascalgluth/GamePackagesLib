cmake_minimum_required(VERSION 3.15)
project(gamepackages CXX)

set(CMAKE_CXX_STANDARD 17)
if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

file(GLOB_RECURSE SOURCES Source/*.cpp)

add_library(gamepackages STATIC ${SOURCES})
set_target_properties(gamepackages PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(gamepackages PUBLIC GPKG_BUILD_LIBRARY)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(ZLIB REQUIRED)
find_package(cryptopp REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ZLIB::ZLIB
        cryptopp::cryptopp
    )

install(TARGETS gamepackages
    EXPORT GameEngine2Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# .
