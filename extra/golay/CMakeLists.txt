add_library(golay
  ${CMAKE_CURRENT_SOURCE_DIR}/src/golay.c
  )

target_include_directories(golay
  PUBLIC include
  )

add_subdirectory(test)

install(TARGETS golay
  DESTINATION lib
)
