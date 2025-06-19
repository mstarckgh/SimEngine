cmake_minimum_required(VERSION 3.10)
project(SimEngine)

set(CMAKE_CXX_STANDARD 20)

# GLFW
add_subdirectory(external/glfw)

# Include Pfade
include_directories(
    external/glfw/include
    external/glad/include
    external/glm
    external/stb
    include
)

# Quellen automatishc einsammeln
file(GLOB_RECURSE SOURCES
    src/*cpp
    tests/*cpp
    external/glad/src/glad.c
)
add_executable(SimEngine ${SOURCES})

# Link gegen GLFW und OpenGL
target_link_libraries(SimEngine glfw ${CMAKE_DL_LIBS})

# Kopiere Shader-Verzeichnis ins Build-Output
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

