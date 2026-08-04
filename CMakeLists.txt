cmake_minimum_required(VERSION 3.16)
project(LearnOpenGLTutorials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencias del sistema (OpenGL y GLFW)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include(FetchContent)

FetchContent_Declare(
  render_lib
  GIT_REPOSITORY "https://github.com/guayaba-dev/SimpleEngine"
  GIT_TAG "v0.1.0"
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


FetchContent_MakeAvailable(render_lib)


function (add_chapter TARGET_NAME SOURCE_FILE)
  add_executable(${TARGET_NAME} ${SOURCE_FILE})

  target_link_libraries(${TARGET_NAME} PRIVATE
    core
    vendor
    GL
    glfw
  )
endfunction()


add_chapter(basicLighting basicLight/basicLighting.cpp)
add_chapter(materials materials/material.cpp)
