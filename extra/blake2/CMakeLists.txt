add_library(blake2
  src/blake2b-ref.c
  src/blake2bp-ref.c
  src/blake2s-ref.c
  src/blake2sp-ref.c
  src/blake2xb-ref.c
  src/blake2xs-ref.c
  )

target_include_directories(blake2
  PUBLIC include
  )

install(TARGETS blake2
  DESTINATION lib
)
