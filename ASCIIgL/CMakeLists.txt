# ASCIIgL - A Console 3D Rendering Engine

cmake_minimum_required(VERSION 3.16)
project(ASCIIgL)

# Set Assimp paths
set(ASSIMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT}/include")
set(ASSIMP_LIB_DIR "${ASSIMP_ROOT}")

# Make glm accessible as an interface target
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm")

# Find Assimp library
find_library(ASSIMP_LIB NAMES assimp-vc143-mt PATHS "${ASSIMP_LIB_DIR}" REQUIRED)

# Create the ASCIIgL library
add_library(ASCIIgL
    src/renderer/Renderer.cpp
    src/renderer/Screen.cpp
    src/renderer/Vertex.cpp

    src/engine/CollisionUtil.cpp
    src/engine/Texture.cpp
    src/engine/Camera2D.cpp
    src/engine/Camera3D.cpp
    src/engine/GameObj.cpp
    src/engine/Mesh.cpp
    src/engine/Model.cpp
    src/engine/Logger.cpp

    src/util/MathUtil.cpp

    src/vendor/stb_image/stb_image.cpp
)

# Set C++ standard for ASCIIgL
set_target_properties(ASCIIgL PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Public include directories for consumers
target_include_directories(ASCIIgL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    ${ASSIMP_INCLUDE_DIR}
)

# Link glm as an interface target
target_link_libraries(ASCIIgL PUBLIC glm)

# Link Assimp library
target_link_libraries(ASCIIgL PRIVATE ${ASSIMP_LIB})

# Link winmm on Windows
if(WIN32)
    target_link_libraries(ASCIIgL PUBLIC winmm)
endif()

# Preprocessor definitions
target_compile_definitions(ASCIIgL PUBLIC
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

# Set output directories for Debug and Release builds
set_target_properties(ASCIIgL PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)