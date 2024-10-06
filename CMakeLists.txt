cmake_minimum_required(VERSION 3.6)

project(
  ExplodeForExplode
  DESCRIPTION "Explode, make money, buy TNT and explode again!"
  VERSION 0.9.0
  LANGUAGES CXX
)

add_compile_options(-Wextra -Wpedantic -Werror)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENGL REQUIRED glfw3 glew)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
