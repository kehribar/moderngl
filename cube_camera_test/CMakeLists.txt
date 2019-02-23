# -----------------------------------------------------------------------------
# 
# -----------------------------------------------------------------------------

# ...
cmake_minimum_required(VERSION 3.0)
project(cube_camera_test)
 
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/glm/)

# ...
file(GLOB SOURCES *.cpp *.c *.h)
add_executable(cube_camera_test ${SOURCES})

# ...
target_link_libraries(cube_camera_test libimgui GL dl glfw)
