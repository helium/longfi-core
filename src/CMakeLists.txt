add_library(lfe
  lfe.c
  lfe_dg_des.c
  )

target_link_libraries(lfe
  PRIVATE golay cursor cursor_varint
  )

target_include_directories(lfe
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  )
