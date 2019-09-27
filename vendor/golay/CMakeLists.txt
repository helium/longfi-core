add_library(golay
  ${CMAKE_CURRENT_SOURCE_DIR}/src/golay.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/golay23.c
  )

target_include_directories(golay
  PUBLIC include
  )

add_subdirectory(test)
