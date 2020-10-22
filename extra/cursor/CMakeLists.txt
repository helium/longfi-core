cmake_minimum_required(VERSION 3.1)

project(Cursor
  VERSION 1.0
  LANGUAGES C
  )

include(CTest)
include(CheckCSourceCompiles)

check_c_source_compiles("
typedef int __attribute__((__may_alias__)) int_a;
int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;
}
  "
  HAVE_ATTRIBUTE_MAY_ALIAS
  )

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(include)

if(${BUILD_TESTING})
add_subdirectory(tests)
add_subdirectory(examples)
endif(${BUILD_TESTING})

