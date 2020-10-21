add_executable(test_cursor test.c)
target_link_libraries(test_cursor cursor)

if(HAVE_ATTRIBUTE_MAY_ALIAS)
  target_compile_definitions(test_cursor
    PRIVATE HAVE_ATTRIBUTE_MAY_ALIAS
    )
endif(HAVE_ATTRIBUTE_MAY_ALIAS)

add_test(NAME test_cursor COMMAND test_cursor)
