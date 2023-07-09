cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../../Project/IncludeTest/bin)
include_directories(../../Library)

file(GLOB_RECURSE LIB_HEADERS RELATIVE "${CMAKE_SOURCE_DIR}/../Library/PAX_SAPIENTICA" "${CMAKE_SOURCE_DIR}/../Library/PAX_SAPIENTICA/*.hpp")

set(TEST_SOURCE_DIR "${CMAKE_SOURCE_DIR}/IncludeTest/source")

foreach(HEADER ${LIB_HEADERS})
  string(REPLACE ".hpp" "IncludeTest.cpp" TEST_NAME ${HEADER})
  set(TEST_PATH "${TEST_SOURCE_DIR}/${TEST_NAME}")

  if(NOT EXISTS ${TEST_PATH})
    file(WRITE ${TEST_PATH} "#include <PAX_SAPIENTICA/${HEADER}>\n\nint main(){}\n")
  endif()
endforeach()

file(GLOB_RECURSE TEST_SOURCES "${TEST_SOURCE_DIR}/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})

  string(REPLACE "IncludeTest.cpp" ".hpp" LIBRARY_PATH ${TEST_SOURCE})
  string(REPLACE "${TEST_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/../Library/PAX_SAPIENTICA/" LIBRARY_PATH ${LIBRARY_PATH})

  if(NOT EXISTS ${LIBRARY_PATH})
    file(REMOVE ${TEST_SOURCE})
    continue()
  endif()

  string(REPLACE "${TEST_SOURCE_DIR}/" "" TARGET_NAME ${TEST_SOURCE})
  string(REPLACE ".cpp" "" TARGET_NAME ${TARGET_NAME})
  string(REPLACE "/" "_" TARGET_NAME ${TARGET_NAME})

  string(REPLACE "${TEST_SOURCE_DIR}/" "source/" TEST_SOURCE ${TEST_SOURCE})

  add_executable(${TARGET_NAME} ${TEST_SOURCE})
endforeach()
