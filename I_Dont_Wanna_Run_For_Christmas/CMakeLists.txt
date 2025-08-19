cmake_minimum_required(VERSION 3.16)
project(I_Dont_Wanna_Run_For_Christmas)

# Enable verbose linker output for MSVC builds
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /VERBOSE")
endif()

# Set ASCIIgLEngine paths
set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../ASCIIgLEngine")
set(ENGINE_INCLUDE_DIR "${ENGINE_ROOT}/src")

# Multi-config generator support (Visual Studio, etc.)
set(ENGINE_LIB_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../ASCIIgLEngine/build/lib/Debug")
set(ENGINE_LIB_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../ASCIIgLEngine/build/lib/Release")

# Add executable
add_executable(ChristmasGame
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
)

# Include directories
include_directories(
    ${ENGINE_ROOT}/src
    ${ENGINE_ROOT}/src/vendor
    ${ENGINE_ROOT}/src/vendor/glm
    ${ENGINE_ROOT}/src/libs/assimp/include
)

# Link ASCIIgLEngine library
message(STATUS "Assimp lib search path: ${ENGINE_ROOT}/src/libs/assimp")

# Print linker search directories after link_directories
get_property(LINK_DIRS DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} PROPERTY LINK_DIRECTORIES)
message(STATUS "Linker search directories: ${LINK_DIRS}")

# Find and link correct engine library for each config
find_library(ASCIIGLENGINE_LIB_DEBUG NAMES ASCIIgLEngine.lib PATHS "${ENGINE_LIB_DIR_DEBUG}" REQUIRED)
find_library(ASCIIGLENGINE_LIB_RELEASE NAMES ASCIIgLEngine.lib PATHS "${ENGINE_LIB_DIR_RELEASE}" REQUIRED)

set(ASSIMP_LIB_PATH "${ENGINE_ROOT}/src/libs/assimp/assimp-vc143-mt.lib")
message(STATUS "Assimp full library path: ${ASSIMP_LIB_PATH}")

target_link_libraries(ChristmasGame PRIVATE
    "$<$<CONFIG:Debug>:${ASCIIGLENGINE_LIB_DEBUG}>"
    "$<$<CONFIG:Release>:${ASCIIGLENGINE_LIB_RELEASE}>"
    "${ASSIMP_LIB_PATH}" winmm
)

# Set C++ standard
set_target_properties(ChristmasGame PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Set output directories for Debug and Release
set_target_properties(ChristmasGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)

# Copy Assimp DLL after build (Windows only)
if(WIN32)
    add_custom_command(TARGET ChristmasGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ENGINE_ROOT}/src/libs/assimp/assimp-vc143-mt.dll"
            $<TARGET_FILE_DIR:ChristmasGame>
    )
endif()

# Copy resources (e.g., res folder) to output directory after build
add_custom_command(TARGET ChristmasGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:ChristmasGame>/res"
)