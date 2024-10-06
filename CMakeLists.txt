cmake_minimum_required(VERSION 3.6)

project(
  ExplodeForExplode
  DESCRIPTION "Explode, make money, buy TNT and explode again!"
  VERSION 0.9.0
  LANGUAGES CXX
)

add_compile_options(-Wextra -Wpedantic -Werror)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.cpp)
