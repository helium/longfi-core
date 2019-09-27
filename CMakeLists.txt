cmake_minimum_required(VERSION 3.1)
enable_testing()

project(LongFi
  VERSION 1.0
  LANGUAGES C
  )

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(vendor)
