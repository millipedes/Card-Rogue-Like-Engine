cmake_minimum_required(VERSION 3.25)
project(CardRL)

################
# Core Library #
################
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SRC_FILES
  ${SRC_DIR}/core/game_state.c
  ${SRC_DIR}/parsing/parsing.c
  ${SRC_DIR}/parsing/parsing_self_card.c
)

include_directories(
  ${SRC_DIR}
)

find_package(Curses REQUIRED)

add_library(CardRLCore ${SRC_FILES})

target_link_libraries(CardRLCore
  PRIVATE
  curses
)

add_executable(card_rl ${SRC_DIR}/main.c)

target_link_libraries(card_rl
  PRIVATE
  CardRLCore
)

option(BUILD_TESTS "Build Test Suite" ON)

if(BUILD_TESTS)
  add_subdirectory(./test)
endif()
