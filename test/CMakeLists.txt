cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
set(TEST_SRC_FILES
  ${TEST_DIR}/ParsingTest.cpp
)
add_executable(tests ${TEST_SRC_FILES} ${TEST_DIR}/main.cpp)
target_link_libraries(tests PRIVATE CardRLCore gtest gtest_main pthread)

