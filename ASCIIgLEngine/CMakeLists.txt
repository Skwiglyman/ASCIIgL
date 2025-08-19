# ASCIIgLEngine - A Console 3D Rendering Engine

cmake_minimum_required(VERSION 3.16)
project(ASCIIgLEngine)

# Set Assimp paths
set(ASSIMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/assimp")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT}/include")
set(ASSIMP_LIB_DIR "${ASSIMP_ROOT}")

# Find Assimp library
find_library(ASSIMP_LIB NAMES assimp-vc143-mt PATHS "${ASSIMP_LIB_DIR}" REQUIRED)

# Create the ASCIIgLEngine library
add_library(ASCIIgLEngine
    src/renderer/ASCIIgLEngine.cpp
    src/renderer/Screen.cpp
    src/renderer/Texture.cpp
    src/engine/Camera2D.cpp
    src/engine/Camera3D.cpp
    src/engine/GameObj.cpp
    src/engine/Mesh.cpp
    src/engine/Model.cpp
    src/engine/Logger.cpp
    src/vendor/stb_image/stb_image.cpp
)

# Set C++ standard for ASCIIgLEngine
set_target_properties(ASCIIgLEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(ASCIIgLEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/glm
)

# Add Assimp include directory
target_include_directories(ASCIIgLEngine PUBLIC
    ${ASSIMP_INCLUDE_DIR}
)

# Link libraries
if(WIN32)
    target_link_libraries(ASCIIgLEngine PUBLIC winmm)
endif()

target_link_libraries(ASCIIgLEngine PRIVATE assimp)
target_link_libraries(ASCIIgLEngine PRIVATE ${ASSIMP_LIB})

# Preprocessor definitions
target_compile_definitions(ASCIIgLEngine PUBLIC
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

# Set output directories for Debug and Release builds
set_target_properties(ASCIIgLEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)