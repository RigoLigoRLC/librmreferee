
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

# tests
file(GLOB RMREFEREE_TEST_SRC test/*.cpp)
add_executable(${PROJECT_NAME}_test ${RMREFEREE_TEST_SRC})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
add_test(RunTest ${PROJECT_NAME}_test)
set_tests_properties(RunTest PROPERTIES FAIL_REGULAR_EXPRESSION "^FAIL:")
