# ============================================================================
# I Don't Wanna Run For Christmas - Demo Game
# ============================================================================

cmake_minimum_required(VERSION 3.16)
project(I_Dont_Wanna_Run_For_Christmas VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# Project Configuration
# ============================================================================

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Debug configuration for MSVC
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /VERBOSE")
endif()

# ============================================================================
# Engine Integration
# ============================================================================

# ASCIIgL Engine paths
set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../ASCIIgL")

# Use pre-built ASCIIgL library (faster builds) - configuration-aware
set(ASCIIgL_LIB_DIR_DEBUG "${ENGINE_ROOT}/build/lib/Debug")
set(ASCIIgL_LIB_DIR_RELEASE "${ENGINE_ROOT}/build/lib/Release")
set(ASCIIgL_INCLUDE_DIR "${ENGINE_ROOT}/include")
set(ASCIIgL_VENDOR_DIR "${ENGINE_ROOT}/vendor")

# Set up GLM as interface library (header-only, needed for engine API)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${ASCIIgL_VENDOR_DIR}")

# Find the pre-built ASCIIgL library for the current configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR (NOT CMAKE_BUILD_TYPE AND CMAKE_CONFIGURATION_TYPES))
    find_library(ASCIIgL_LIBRARY_DEBUG
        NAMES ASCIIgL 
        PATHS "${ASCIIgL_LIB_DIR_DEBUG}" 
        REQUIRED
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR (NOT CMAKE_BUILD_TYPE AND CMAKE_CONFIGURATION_TYPES))
    find_library(ASCIIgL_LIBRARY_RELEASE
        NAMES ASCIIgL 
        PATHS "${ASCIIgL_LIB_DIR_RELEASE}"
    )
endif()

# Find Assimp import library (required for linking)
find_library(ASSIMP_LIBRARY 
    NAMES assimp-vc143-mt 
    PATHS "${ENGINE_ROOT}/libs/assimp" 
    REQUIRED
)

# Create imported target for ASCIIgL with configuration-specific locations
add_library(ASCIIgL STATIC IMPORTED)

# Set properties based on what we found
if(ASCIIgL_LIBRARY_DEBUG)
    set_target_properties(ASCIIgL PROPERTIES
        IMPORTED_LOCATION_DEBUG "${ASCIIgL_LIBRARY_DEBUG}"
    )
endif()

if(ASCIIgL_LIBRARY_RELEASE)
    set_target_properties(ASCIIgL PROPERTIES
        IMPORTED_LOCATION_RELEASE "${ASCIIgL_LIBRARY_RELEASE}"
    )
endif()

# Set common properties
set_target_properties(ASCIIgL PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ASCIIgL_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "glm;${ASSIMP_LIBRARY}"
)

# ============================================================================
# Game Executable
# ============================================================================

# Create the game executable
add_executable(ChristmasGame
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    "${ASCIIgL_VENDOR_DIR}/stb_image/stb_image.cpp"  # Include stb_image source directly
)

# ============================================================================
# Linking
# ============================================================================

# Link to ASCIIgL engine (automatically provides all dependencies)
target_link_libraries(ChristmasGame PRIVATE ASCIIgL)

# ============================================================================
# Output Configuration
# ============================================================================

# Set output directories for different build configurations
set_target_properties(ChristmasGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)

# ============================================================================
# Post-Build Steps
# ============================================================================

# Copy Assimp DLL to output directory (Windows only)
if(WIN32)
    add_custom_command(TARGET ChristmasGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ENGINE_ROOT}/libs/assimp/assimp-vc143-mt.dll"
            $<TARGET_FILE_DIR:ChristmasGame>
        COMMENT "Copying Assimp DLL to output directory"
    )
endif()

# Copy game resources to output directory
add_custom_command(TARGET ChristmasGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:ChristmasGame>/res"
    COMMENT "Copying game resources to output directory"
)