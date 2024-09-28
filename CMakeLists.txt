cmake_minimum_required(VERSION 3.29)
project(Traceroute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TRACEROUTE_EXECUTE_NAME traceroute)
set(TRACEROUTE_TESTS_NAME traceroute_tests)

add_executable(${TRACEROUTE_EXECUTE_NAME} src/main.cpp src/Traceroute.cpp)


add_compile_options(-pedantic -Werror -Wextra)
add_link_options(-pedantic -Werror -Wextra)

# Build with sanitizers
string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type)
if (build_type STREQUAL "san")
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# Add google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(${TRACEROUTE_TESTS_NAME} test/traceroute_test.cpp src/Traceroute.cpp)

target_link_libraries(${TRACEROUTE_TESTS_NAME} Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

enable_testing()
add_test(NAME ${TRACEROUTE_TESTS_NAME} COMMAND ${TRACEROUTE_TESTS_NAME})

