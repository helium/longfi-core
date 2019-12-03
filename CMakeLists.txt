##########################################################################
# Project Setup                                                          #
##########################################################################
cmake_minimum_required(VERSION 3.11)

project(LongFiCore
  # Remember to update `src/text_version.c` when bumping the version,
  # otherwise tests/CI will fail.
  VERSION   0.3.0
  LANGUAGES C
  )

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)


##########################################################################
# Static Analysis                                                        #
##########################################################################
find_program(CPPCHECK NAMES cppcheck)
if(CPPCHECK)
  message(STATUS "Found cppcheck: ${CPPCHECK}")
  set(CMAKE_C_CPPCHECK ${CPPCHECK}
    --enable=all
    --suppress=missingIncludeSystem
    --suppress=unusedFunction
    --quiet
    --std=c${CMAKE_C_STANDARD}
    --template=gcc
    )
endif()


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
  unset(CMAKE_C_CPPCHECK)
  add_subdirectory(${cursor_SOURCE_DIR} ${cursor_BINARY_DIR})
endif()
