add_library(lfe
  lfe.c
  lfe_dg_des.c
  lfe_dg_ser.c
  )

target_link_libraries(lfe
  PRIVATE golay cursor cursor_varint
  )

target_include_directories(lfe
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  )
