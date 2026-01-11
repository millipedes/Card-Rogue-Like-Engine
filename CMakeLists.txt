cmake_minimum_required(VERSION 3.25)
project(CardRL)

################
# Core Library #
################
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_SRC_FILES
  ${SRC_DIR}/core/game_state.c
  ${SRC_DIR}/core/self_state.c
  ${SRC_DIR}/parsing/parsing.c
  ${SRC_DIR}/parsing/parsing_self_card.c
)

include_directories(
  ${SRC_DIR}
)

find_package(Curses REQUIRED)

add_library(CardRLCore ${LIB_SRC_FILES})

set(EXE_SRC_FILES
  ${SRC_DIR}/graphics/battle_view.c
  ${SRC_DIR}/graphics/core_graphics.c
  ${SRC_DIR}/graphics/self_view.c
)

add_executable(card_rl ${EXE_SRC_FILES} ${SRC_DIR}/main.c)

target_link_libraries(card_rl
  PRIVATE
  CardRLCore
  curses
)

option(BUILD_TESTS "Build Test Suite" ON)

if(BUILD_TESTS)
  add_subdirectory(./test)
endif()
