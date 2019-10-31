cmake_minimum_required(VERSION 3.10.2)

project(flang)

enable_testing()

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall \
  -Wextra \
  -Wpedantic \
  -Werror \
  -pipe \
  -fexceptions \
  -fsanitize=address \
  -fasynchronous-unwind-tables \
  -fstack-protector-strong"
)

set(SOURCES
  ${PROJECT_SOURCE_DIR}/src/AstWalker.cpp
  ${PROJECT_SOURCE_DIR}/src/Error.cpp
  ${PROJECT_SOURCE_DIR}/src/Parser.cpp
  ${PROJECT_SOURCE_DIR}/src/SemanticAnalyzer.cpp
  ${PROJECT_SOURCE_DIR}/src/StringReader.cpp
  ${PROJECT_SOURCE_DIR}/src/Token.cpp
  ${PROJECT_SOURCE_DIR}/src/TokenBuffer.cpp
  ${PROJECT_SOURCE_DIR}/src/Tokenizer.cpp
)

set(TEST_SOURCES)

message("CMAKE_CXX_STANDARD: " ${CMAKE_CXX_STANDARD})
message("CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message("SOURCES: " ${SOURCES})
message("TEST_SOURCES: " ${TEST_SOURCES})

add_executable(flang
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${SOURCES})

add_executable(flang_frontend_tester
  ${PROJECT_SOURCE_DIR}/test/frontend_tester.cpp
  ${SOURCES}
  ${TEST_SOURCES})

set(FRONTEND_TEST_DATA_DIR ${PROJECT_SOURCE_DIR}/data/test/frontend)

add_test(pass1 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass1.txt none)
add_test(pass2 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass2.txt none)
add_test(pass3 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass3.txt none)
add_test(pass4 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass4.txt none)
add_test(pass5 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass5.txt none)
add_test(pass6 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass6.txt none)
add_test(pass7 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass7.txt none)
add_test(pass8 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass8.txt none)