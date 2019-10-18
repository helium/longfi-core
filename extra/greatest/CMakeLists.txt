add_library(greatest INTERFACE)
target_include_directories(greatest INTERFACE include)

install(TARGETS greatest
  DESTINATION lib
)