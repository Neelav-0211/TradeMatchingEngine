find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Test executable
file(GLOB TEST_SOURCES "*.cpp")
add_executable(test_matching_engine ${TEST_SOURCES} 
                                   "${CMAKE_SOURCE_DIR}/src/OrderBook.cpp"
                                   "${CMAKE_SOURCE_DIR}/src/MatchingEngine.cpp")

# Link with Google Test and the main library
target_link_libraries(test_matching_engine gtest gtest_main)

# Add the test
add_test(NAME test_matching_engine COMMAND test_matching_engine)
