
cmake_minimum_required(VERSION 3.21)

project(rmreferee LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20 REQUIRED)

set(STD_STREAM_LOGGER ON)

enable_testing()

if(STD_STREAM_LOGGER)
    add_definitions(-DSTD_STREAM_LOGGER)
endif()

# librmreferee
file(GLOB_RECURSE RMREFEREE_SRC src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${RMREFEREE_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC inc)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(RMREFEREE_LIBS ${PROJECT_NAME} PARENT_SCOPE)
endif()

# tests
file(GLOB RMREFEREE_TEST_SRC test/*.cpp)
add_executable(${PROJECT_NAME}_test ${RMREFEREE_TEST_SRC})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
add_test(RunTest ${PROJECT_NAME}_test)
set_tests_properties(RunTest PROPERTIES FAIL_REGULAR_EXPRESSION "^FAIL:")
