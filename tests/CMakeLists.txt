add_executable(test_parser test_parser.c)
target_link_libraries(test_parser PRIVATE ${CMARK_LIBRARIES})

add_executable(test_renderer test_renderer.c)
target_link_libraries(test_renderer PRIVATE ${CMARK_LIBRARIES})

add_test(NAME parser_test COMMAND test_parser)
add_test(NAME renderer_test COMMAND test_renderer)