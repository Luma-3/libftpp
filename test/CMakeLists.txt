cmake_minimum_required(VERSION 3.14)
project(libftpp_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(libftpp_test
  pool_test.cc
)

target_include_directories(libftpp_test PRIVATE 
  ../inc
)

target_link_libraries(libftpp_test PRIVATE
  gtest_main
  ${CMAKE_SOURCE_DIR}/../libftpp.a
)

include(GoogleTest)
gtest_discover_tests(libftpp_test)
