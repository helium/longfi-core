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
    --inline-suppr
    --quiet
    --std=c${CMAKE_C_STANDARD}
    --suppress=missingIncludeSystem
    --suppress=unusedFunction
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
