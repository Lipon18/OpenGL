# --- CMakeLists.txt for OpenGL Project ---

cmake_minimum_required(VERSION 3.20.0)

project(OpenGL C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/glad.c 
    src/stb.cpp
    src/pch.cpp

    src/camera/Camera.cpp
    src/input/Input.cpp
    src/input/Keyboard.cpp
    src/input/Mouse.cpp
    src/mesh/Mesh.cpp
    src/mesh/Model.cpp
    src/renderer/lighting/DirLight.cpp
    src/renderer/lighting/Light.cpp
    src/renderer/lighting/PointLight.cpp
    src/renderer/lighting/SpotLight.cpp
    src/window/Window.cpp
)

set(HEADERS
    src/pch.h
    src/camera/Camera.h
    src/input/Input.h
    src/input/Keyboard.h
    src/input/Mouse.h
    src/mesh/Mesh.h
    src/mesh/Model.h
    src/shaders/Shader.cpp
    src/renderer/lighting/DirLight.h
    src/renderer/lighting/Light.h
    src/renderer/lighting/PointLight.h
    src/renderer/lighting/SpotLight.h
    src/shaders/GL_ShaderSource.h
    src/window/Window.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if(MSVC)
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>")
endif()

# --- Setting up Include Directories (linking/include) ---
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include"    # parent of all headers
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include/assimp"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include/glad"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include/GLFW"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include/KHR"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/include/stb"
)

# --- Setting up Library Directories (linking/lib) ---
target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/lib/assimp"
    "${CMAKE_CURRENT_SOURCE_DIR}/linking/lib/GLFW"
)

find_package(OpenGL REQUIRED)

# --- Link Libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    assimp-vc143-mtd
    glfw3
    gdi32
    user32
    shell32
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT")

add_compile_definitions(GLAD_GL)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/assimp-vc143-mtd.dll"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")