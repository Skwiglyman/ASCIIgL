# ASCIIgL - A Console 3D Rendering Engine

cmake_minimum_required(VERSION 3.16)
project(ASCIIgL VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# Project Configuration
# ============================================================================

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# ============================================================================
# Dependencies Setup
# ============================================================================

# GLM (Header-only math library)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
)

# Make GLM available to parent projects when using add_subdirectory
set_target_properties(glm PROPERTIES EXPORT_NAME glm)
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # We're being built as a subdirectory, create an alias
    add_library(ASCIIgL::glm ALIAS glm)
endif()

# Assimp (3D model loading library)
set(ASSIMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT}/include")
set(ASSIMP_LIB_DIR "${ASSIMP_ROOT}")

find_library(ASSIMP_LIB 
    NAMES assimp-vc143-mt 
    PATHS "${ASSIMP_LIB_DIR}" 
    REQUIRED
)

# ============================================================================
# Source Files
# ============================================================================

# Automatically find all source files
file(GLOB_RECURSE ASCIIgL_SOURCES
    "src/renderer/*.cpp"
    "src/engine/*.cpp"
    "src/util/*.cpp"
    "src/vendor/stb_image/stb_image.cpp"
)

# ============================================================================
# Library Target
# ============================================================================

# Create the main library
add_library(ASCIIgL ${ASCIIgL_SOURCES})

# ============================================================================
# Include Directories
# ============================================================================

# Public includes (available to consumers)
target_include_directories(ASCIIgL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ASSIMP_INCLUDE_DIR}
)

# Private includes (internal use only)
target_include_directories(ASCIIgL PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

# ============================================================================
# Linking
# ============================================================================

# Public dependencies (exposed to consumers)
target_link_libraries(ASCIIgL PUBLIC 
    glm
)

# Private dependencies (internal use only)
target_link_libraries(ASCIIgL PRIVATE 
    ${ASSIMP_LIB}
)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(ASCIIgL PUBLIC winmm)
endif()

# ============================================================================
# Compiler Definitions
# ============================================================================

target_compile_definitions(ASCIIgL PUBLIC
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

# ============================================================================
# Output Directories
# ============================================================================

set_target_properties(ASCIIgL PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)