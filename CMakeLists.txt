cmake_minimum_required(VERSION 3.0)
project(autot)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread")

add_executable(${PROJECT_NAME} main.cpp)

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})

add_definitions(-DCURL_STATICLIB)
message("build type: " ${CMAKE_BUILD_TYPE})
