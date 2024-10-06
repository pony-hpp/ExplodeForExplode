set(EFE_TESTS ${PROJECT_NAME}Tests)

find_package(GTest REQUIRED)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(${EFE_TESTS} ${SRC})
target_link_libraries(${EFE_TESTS} ${EFE_COMMON} ${GTEST_LIBRARIES})
