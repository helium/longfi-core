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

add_executable(test_serdes test_serdes.c)
target_link_libraries(test_serdes lfe cursor greatest)
add_test(NAME test_serdes COMMAND test_serdes)
