##########################################################################
# Project Setup                                                          #
##########################################################################
cmake_minimum_required(VERSION 3.11)

project(LongFiCore
  VERSION   1.0
  LANGUAGES C
  )

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

##########################################################################
# Local                                                                  #
##########################################################################
add_subdirectory(docs)
add_subdirectory(extra)
add_subdirectory(include)
add_subdirectory(src)


##########################################################################
# External                                                               #
##########################################################################
include(FetchContent)
FetchContent_Declare(
  cursor
  GIT_REPOSITORY https://github.com/JayKickliter/cursor.git
  GIT_TAG        0.8.2
  )
FetchContent_GetProperties(cursor)
if(NOT cursor_POPULATED)
  FetchContent_Populate(cursor)
  add_subdirectory(${cursor_SOURCE_DIR} ${cursor_BINARY_DIR})
endif()
