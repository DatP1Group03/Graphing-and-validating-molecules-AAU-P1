
add_executable(test_SMILESNODE test_SMILESNODE.c)
target_link_libraries(test_SMILESNODE test_SMILESNODE mtest)

discover_tests(test_SMILESNODE)
