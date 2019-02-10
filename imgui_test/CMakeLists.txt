# -----------------------------------------------------------------------------
# 
# -----------------------------------------------------------------------------

# ...
cmake_minimum_required(VERSION 3.0)
project(imgui_test)
 
# ...
set(IMGUI_BASE_FOLDER "../external/imgui/imgui-1.67")
set(IMGUI_LIBPATH "../../external/imgui/build/libimgui.a")
include_directories(${IMGUI_BASE_FOLDER})
include_directories(${IMGUI_BASE_FOLDER}/build)
include_directories(${IMGUI_BASE_FOLDER}/examples)
include_directories(${IMGUI_BASE_FOLDER}/examples/libs/gl3w)
add_library(what UNKNOWN IMPORTED)
set_property(TARGET what PROPERTY IMPORTED_LOCATION ${IMGUI_LIBPATH})

# ...
file(GLOB SOURCES "*.cpp" "*.c")
add_executable(imgui_test ${SOURCES})

# ...
target_link_libraries(imgui_test what GL dl glfw)
