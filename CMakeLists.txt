cmake_minimum_required(VERSION 3.1)

project(WebVisualize VERSION 1.0 DESCRIPTION "3D visualizer for the web." LANGUAGES CXX)

configure_file ("${PROJECT_SOURCE_DIR}/Config.h.in" "${PROJECT_BINARY_DIR}/Config.h")
include_directories("${PROJECT_BINARY_DIR}")

# C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenGL
if(NOT ${EMSCRIPTEN})
	find_package(glfw3 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)

	find_package(GLM QUIET)
	if (!GLM_FOUND)
		set(GLM_INCLUDE_DIRS "/usr/include/glm/")
		message(WARNING "Unable to find glm. Setting include path to ${GLM_INCLUDE_DIRS}.")
	endif(!GLM_FOUND)
endif()


set(SOURCE src/main.cpp src/context.cpp)
# set(SHADERS shaders/FragmentShader.glsl shaders/VertexShader.glsl)
file(GLOB WEB_FILES web/*)
file(COPY ${WEB_FILES} DESTINATION "${PROJECT_BINARY_DIR}/web")
set(SHADERS web/index.html)

add_executable(webvisualize ${SOURCE} ${SHADERS})

target_include_directories(webvisualize PUBLIC include)
target_include_directories(webvisualize PRIVATE ${GLEW_INCLUDE_DIRS})
target_include_directories(webvisualize PRIVATE ${GLM_INCLUDE_DIRS})

target_link_libraries(webvisualize PRIVATE glfw)
target_link_libraries(webvisualize PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(webvisualize PRIVATE ${GLEW_LIBRARIES})
target_link_libraries(webvisualize PRIVATE "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")

set_target_properties(webvisualize PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/web"
)