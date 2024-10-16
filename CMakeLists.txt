cmake_minimum_required(VERSION 3.6)

project(
  ExplodeForExplode
  DESCRIPTION "Explode, make money, buy TNT and explode again!"
  VERSION 0.9.0
  LANGUAGES CXX
)

add_compile_options(-Wextra -Wpedantic -Werror)
add_compile_definitions(EFE_VERSION="${PROJECT_VERSION}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENGL REQUIRED glfw3 glew)
pkg_check_modules(PNG REQUIRED libpng)

file(GLOB_RECURSE SRC src/*.cpp)
list(REMOVE_ITEM SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)
set(EFE_COMMON ${PROJECT_NAME}Common)
add_library(${EFE_COMMON} OBJECT ${SRC})
target_include_directories(${EFE_COMMON} PUBLIC src)
target_link_libraries(${EFE_COMMON} ${OPENGL_LIBRARIES} ${PNG_LIBRARIES})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${EFE_COMMON})

add_subdirectory(tests)
