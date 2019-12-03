add_library(lfc
  lfc.c
  lfc_dg_des.c
  lfc_dg_eq.c
  lfc_dg_ser.c
  lfc_fingerprint.c
  lfc_version.c
  )

target_link_libraries(lfc
  PRIVATE golay cursor cursor_varint blake2
  )

target_include_directories(lfc
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
  )

if(${BUILD_TESTING})
  add_executable(test_serdes test_serdes.c)
  target_link_libraries(test_serdes lfc cursor greatest)
  add_test(NAME test_serdes COMMAND test_serdes)

  add_executable(test_tx_rx test_tx_rx.c)
  target_link_libraries(test_tx_rx lfc cursor greatest)
  add_test(NAME test_tx_rx COMMAND test_tx_rx)

  add_executable(test_fingerprinting test_fingerprinting.c)
  target_link_libraries(test_fingerprinting lfc cursor greatest)
  add_test(NAME test_fingerprinting COMMAND test_fingerprinting)

  add_executable(test_version test_version.c)
  target_link_libraries(test_version lfc)
  add_test(NAME test_version COMMAND test_version)
endif(${BUILD_TESTING})

install(TARGETS lfc
  DESTINATION lib
)

##########################################################################
# Generate `version.h`                                                   #
##########################################################################
set(VERSION_H_IN ${CMAKE_CURRENT_SOURCE_DIR}/lfc_version.h.in)
set(VERSION_H_OUT ${CMAKE_CURRENT_BINARY_DIR}/lfc_version.h)
configure_file(${VERSION_H_IN} ${VERSION_H_OUT} @ONLY)
