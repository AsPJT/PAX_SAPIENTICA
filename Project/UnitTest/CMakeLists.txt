set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary/googletest/googletest/include)

set(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

file(GLOB_RECURSE TEST_SOURCES "${TEST_SOURCE_DIR}/*.cpp")
foreach(TEST_SOURCE ${TEST_SOURCES})
    string(REPLACE "${TEST_SOURCE_DIR}/" "" TARGET_NAME ${TEST_SOURCE})
    string(REPLACE ".cpp" "" TARGET_NAME ${TARGET_NAME})
    string(REPLACE "/" "_" TARGET_NAME ${TARGET_NAME})
    string(REPLACE "${TEST_SOURCE_DIR}/" "source/" TEST_SOURCE ${TEST_SOURCE})
    add_executable(${TARGET_NAME} ${TEST_SOURCE})
    target_link_libraries(${TARGET_NAME} gtest_main)
    add_test(
        NAME ${TARGET_NAME} 
        COMMAND ${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    )
    set_tests_properties(${TARGET_NAME} PROPERTIES LABELS "Unit")
endforeach()
