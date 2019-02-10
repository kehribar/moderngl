# -----------------------------------------------------------------------------
# Custom CMake file for creating static library out of imgui
# Configured for OpenGL3 + GLFW
# -----------------------------------------------------------------------------

# ...
cmake_minimum_required(VERSION 3.0)

# ...
set(IMGUI_BASE_FOLDER "./imgui-1.67")

# ...
include_directories(
  ${IMGUI_BASE_FOLDER}
  ${IMGUI_BASE_FOLDER}/examples/libs/gl3w
)

# ...
add_library(
  imgui STATIC
  ${IMGUI_BASE_FOLDER}/imconfig.h
  ${IMGUI_BASE_FOLDER}/imgui.h
  ${IMGUI_BASE_FOLDER}/imgui.cpp
  ${IMGUI_BASE_FOLDER}/imgui_demo.cpp
  ${IMGUI_BASE_FOLDER}/imgui_draw.cpp
  ${IMGUI_BASE_FOLDER}/imgui_widgets.cpp
  ${IMGUI_BASE_FOLDER}/imgui_internal.h
  ${IMGUI_BASE_FOLDER}/imstb_rectpack.h
  ${IMGUI_BASE_FOLDER}/imstb_textedit.h
  ${IMGUI_BASE_FOLDER}/imstb_truetype.h
  ${IMGUI_BASE_FOLDER}/examples/imgui_impl_glfw.h
  ${IMGUI_BASE_FOLDER}/examples/imgui_impl_glfw.cpp
  ${IMGUI_BASE_FOLDER}/examples/libs/gl3w/GL/gl3w.c
  ${IMGUI_BASE_FOLDER}/examples/libs/gl3w/GL/gl3w.h
  ${IMGUI_BASE_FOLDER}/examples/libs/gl3w/GL/glcorearb.h
  ${IMGUI_BASE_FOLDER}/examples/imgui_impl_opengl3.h
  ${IMGUI_BASE_FOLDER}/examples/imgui_impl_opengl3.cpp
)
