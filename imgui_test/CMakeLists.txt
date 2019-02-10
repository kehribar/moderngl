# -----------------------------------------------------------------------------
# 
# -----------------------------------------------------------------------------

# ...
cmake_minimum_required(VERSION 3.0)
project(imgui_test)
 
# ...
set(IMGUI_BASE_FOLDER 
  ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/imgui-1.67
)
set(IMGUI_LIBPATH 
  ${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/build/libimgui.a
)

# ...
include_directories(${IMGUI_BASE_FOLDER})
include_directories(${IMGUI_BASE_FOLDER}/build)
include_directories(${IMGUI_BASE_FOLDER}/examples)
include_directories(${IMGUI_BASE_FOLDER}/examples/libs/gl3w)
add_library(libimgui UNKNOWN IMPORTED)
set_property(TARGET libimgui PROPERTY IMPORTED_LOCATION ${IMGUI_LIBPATH})

# ...
file(GLOB SOURCES *.cpp *.c *.h)
add_executable(imgui_test ${SOURCES})

# ...
target_link_libraries(imgui_test libimgui GL dl glfw)
