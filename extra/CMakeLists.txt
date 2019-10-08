add_subdirectory(blake2)
add_subdirectory(cursor_varint)
add_subdirectory(golay)

if(${BUILD_TESTING})
  add_subdirectory(greatest)
endif(${BUILD_TESTING})
