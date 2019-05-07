cmake_minimum_required(VERSION 3.1)

# OpenGL
if(NOT ${EMSCRIPTEN})
	find_package(glfw3 REQUIRED)
	find_package(OpenGL REQUIRED)
endif()

set(SOURCE
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_widgets.cpp)

add_library(imgui ${SOURCE})

target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(imgui PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(imgui PRIVATE "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
