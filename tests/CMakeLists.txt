include(CTest)

set(TEST_SOURCES
    ${TEST_DIR}/array/test_array.c
)

set(TEST_EXE test_array)

add_executable(${TEST_EXE} ${TEST_SOURCES})
target_link_libraries(${TEST_EXE} dsa)
target_include_directories(${TEST_EXE} PRIVATE ${INCLUDE_DIR})

add_test(NAME array COMMAND ${TEST_EXE})

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
