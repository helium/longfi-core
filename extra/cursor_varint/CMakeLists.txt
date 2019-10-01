add_library(cursor_varint varint.c)
target_link_libraries(cursor_varint cursor greatest)
target_include_directories(cursor_varint PUBLIC include)

add_executable(test_varint test_varint.c)
target_link_libraries(test_varint cursor_varint)

add_test(NAME test_varint COMMAND test_varint)
