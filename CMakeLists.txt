cmake_minimum_required(VERSION 3.1)

project(WebVisualize VERSION 1.0 DESCRIPTION "3D visualizer for the web." LANGUAGES CXX)

add_subdirectory(external)

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

file(GLOB DATA_FILES data/*)
file(COPY ${DATA_FILES} DESTINATION "${PROJECT_BINARY_DIR}/data")

add_executable(webvisualize ${SOURCE} ${SHADERS})

target_include_directories(webvisualize PUBLIC include)
target_include_directories(webvisualize PUBLIC external/imgui)
target_include_directories(webvisualize PRIVATE ${GLEW_INCLUDE_DIRS})
target_include_directories(webvisualize PRIVATE ${GLM_INCLUDE_DIRS})

target_link_libraries(webvisualize PRIVATE glfw)
target_link_libraries(webvisualize PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(webvisualize PRIVATE ${GLEW_LIBRARIES})
target_link_libraries(webvisualize PRIVATE imgui)
target_link_libraries(webvisualize PRIVATE "-s USE_GLFW=3 -s FULL_ES3=1 --preload-file data")

set_target_properties(webvisualize PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/web")