add_library(lfc
  lfc.c
  lfc_dg_des.c
  lfc_dg_eq.c
  lfc_dg_ser.c
  )

target_link_libraries(lfc
  PRIVATE golay cursor cursor_varint blake2
  )

target_include_directories(lfc
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

if(${BUILD_TESTING})
  add_executable(test_serdes test_serdes.c)
  target_link_libraries(test_serdes lfc cursor greatest)
  add_test(NAME test_serdes COMMAND test_serdes)

  add_executable(test_tx_rx test_tx_rx.c)
  target_link_libraries(test_tx_rx lfc cursor greatest)
  add_test(NAME test_tx_rx COMMAND test_tx_rx)
endif(${BUILD_TESTING})
