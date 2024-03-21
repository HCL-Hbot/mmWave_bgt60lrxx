include(CTest)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(BGT60_UNIT_TEST 
    bgt60_driver_unit_test.cpp
)

target_link_libraries(BGT60_UNIT_TEST gtest gmock)

add_test(NAME BGT60_UNIT_TEST COMMAND runUnitTests)
