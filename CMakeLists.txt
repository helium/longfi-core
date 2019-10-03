##########################################################################
# Project Setup                                                          #
##########################################################################
cmake_minimum_required(VERSION 3.11)
enable_testing()

project(LongFi
  VERSION   1.0
  LANGUAGES C
  )

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


##########################################################################
# Local                                                                  #
##########################################################################
add_subdirectory(docs)
add_subdirectory(extra)
add_subdirectory(src)


##########################################################################
# External                                                               #
##########################################################################
include(FetchContent)
FetchContent_Declare(
  cursor
  GIT_REPOSITORY https://github.com/JayKickliter/cursor.git
  GIT_TAG        7a4a5a86130835a86865d6668a6df61be20b912e
  )
FetchContent_GetProperties(cursor)
if(NOT cursor_POPULATED)
  FetchContent_Populate(cursor)
  add_subdirectory(${cursor_SOURCE_DIR} ${cursor_BINARY_DIR})
endif()
