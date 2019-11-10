cmake_minimum_required(VERSION 3.10.2)

project(flang)

enable_testing()

include_directories(include)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "-fexceptions -fsanitize=address -fasynchronous-unwind-tables -fstack-protector-strong -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCES
)

add_executable(flang
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${SOURCES})

#add_executable(flang_frontend_tester
#  ${PROJECT_SOURCE_DIR}/test/frontend_tester.cpp
#  ${SOURCES})

#set(FRONTEND_TEST_DATA_DIR ${PROJECT_SOURCE_DIR}/data/test/frontend)
#
#add_test(pass1 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass1.f none)
#add_test(pass2 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass2.f none)
#add_test(pass3 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass3.f none)
#add_test(pass4 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass4.f none)
#add_test(pass5 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass5.f none)
#add_test(pass6 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass6.f none)
#add_test(pass7 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass7.f none)
#add_test(pass8 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass8.f none)
#add_test(pass9 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass9.f none)
#add_test(pass10 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass10.f none)
#add_test(pass11 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass11.f none)
#add_test(pass12 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass12.f none)
#add_test(pass13 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass13.f none)
#add_test(pass15 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass15.f none)
#add_test(pass16 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass16.f none)
#add_test(pass17 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass17.f none)
#add_test(pass14 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass14.f none)
#add_test(pass18 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/pass18.f none)
#
#add_test(fail_semantic1 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic1.f semantic_analysis)
#add_test(fail_semantic2 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic2.f semantic_analysis)
#add_test(fail_semantic3 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic3.f semantic_analysis)
#add_test(fail_semantic4 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic4.f semantic_analysis)
#add_test(fail_semantic5 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic5.f semantic_analysis)
#add_test(fail_semantic6 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic6.f semantic_analysis)
#add_test(fail_semantic7 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic7.f semantic_analysis)
#add_test(fail_semantic8 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic8.f semantic_analysis)
#add_test(fail_semantic9 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic9.f semantic_analysis)
#add_test(fail_semantic10 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic10.f semantic_analysis)
#add_test(fail_semantic11 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic11.f semantic_analysis)
#add_test(fail_semantic12 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic12.f semantic_analysis)
#add_test(fail_semantic13 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic13.f semantic_analysis)
#add_test(fail_semantic15 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic15.f semantic_analysis)
#add_test(fail_semantic18 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_semantic18.f semantic_analysis)
#
#add_test(fail_parsing1 flang_frontend_tester ${FRONTEND_TEST_DATA_DIR}/fail_parsing1.f parsing)