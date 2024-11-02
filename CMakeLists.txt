cmake_minimum_required(VERSION 3.6)

project(
  ExplodeForExplode
  DESCRIPTION "Explode, make money, buy dynamites and explode again!"
  VERSION 0.9.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
  add_compile_options(-Wextra -Wpedantic -Werror)
endif() # Otherwise don't set any flags, because MSVC is too squeamish.

add_compile_definitions(EFE_VERSION="${PROJECT_VERSION}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(OPENGL REQUIRED glfw3 glew)
  pkg_check_modules(PNG REQUIRED libpng)
else()
  find_package(OpenGL REQUIRED)
  find_package(GLFW3 REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(PNG REQUIRED)
  set(OPENGL_LIBRARIES ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})
endif()

file(GLOB_RECURSE SRC src/*.cpp)
list(REMOVE_ITEM SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)
set(EFE_COMMON ${PROJECT_NAME}Common)
add_library(${EFE_COMMON} OBJECT ${SRC})
target_include_directories(${EFE_COMMON} PUBLIC src)
target_link_libraries(${EFE_COMMON} ${OPENGL_LIBRARIES} ${PNG_LIBRARIES})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${EFE_COMMON})

if(EFE_TESTS)
  add_subdirectory(tests)
endif()
