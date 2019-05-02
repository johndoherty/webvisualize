cmake_minimum_required(VERSION 3.1)
project(Visualize3D)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )

include_directories("${PROJECT_BINARY_DIR}")

# C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenGL
# find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)

# find_package(GLM QUIET)
# if (!GLM_FOUND)
# 	set(GLM_INCLUDE_DIRS "/usr/include/glm/")
# 	message(WARNING "Unable to find glm. Setting include path to ${GLM_INCLUDE_DIRS}.")
# endif(!GLM_FOUND)

set(SOURCE src/main.cpp)
# set(SHADERS shaders/FragmentShader.glsl shaders/VertexShader.glsl)

add_library(visualize3d ${SOURCE} ${SHADERS})

target_include_directories(visualize3d PUBLIC ../include)
target_include_directories(visualize3d PRIVATE ${GLEW_INCLUDE_DIRS})
target_include_directories(visualize3d PRIVATE ${GLM_INCLUDE_DIRS})

target_link_libraries(visualize3d PRIVATE glfw)
target_link_libraries(visualize3d PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(visualize3d PRIVATE ${GLEW_LIBRARIES})
