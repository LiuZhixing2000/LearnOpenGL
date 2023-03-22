cmake_minimum_required (VERSION 3.23)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(LearnOpenGL)
set(CMAKE_CXX_STANDARD 11)

include_directories(/opt/homebrew/Cellar/glfw/3.3.7/include)
# include_directories(/opt/homebrew/Cellar/glew/2.2.0_1/include)
include_directories(include)
link_directories(/opt/homebrew/Cellar/glfw/3.3.7/lib)
link_directories(lib)
# link_directories(/opt/homebrew/Cellar/glew/2.2.0_1/lib)

set(
    SOURCES
    # src/hello_window.cpp
    # src/hello_triangle.cpp
    # src/shader.cpp
    # src/texture.cpp
    # src/transformation.cpp
    # src/coordinate_system.cpp
    # src/camera.cpp
    # src/colors.cpp
    # src/basic_lightings.cpp
    # src/lighting_maps.cpp
    # src/light_casters.cpp
    src/loading_model.cpp
    src/model.h
    src/mesh.h
    src/mesh.cpp
    src/shader_class.h
    src/shader_class.cpp
    src/camera_class.h
    src/glad.c
)

add_executable(LearnOpenGL ${SOURCES})
target_link_libraries(LearnOpenGL GLFW)
target_link_libraries(LearnOpenGL libassimp.5.2.5.dylib)
# target_link_libraries(LearnOpenGL GLEW)