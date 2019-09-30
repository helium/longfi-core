add_library(lfe
  lfe.c
  lfe_dg_parse.c
  )

target_link_libraries(lfe
  PRIVATE golay cursor
  )

target_include_directories(lfe
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  )
