add_library(lfe
  lfe.c
  )

target_include_directories(lfe
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  )
