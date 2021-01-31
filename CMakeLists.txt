cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

project(autot)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread")

file(GLOB SOURCES src/*.h src/*.cpp src/d_fetch/*.h src/d_fetch/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})

add_definitions(-DCURL_STATICLIB)
message("build type: " ${CMAKE_BUILD_TYPE})

#set(OPENSSL_USE_STATIC_LIBS TRUE)
#configure for macos
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)

# Specify the minimum version you require.
find_package (mongoc-1.0 1.7 REQUIRED)

# The "hello_mongoc.c" sample program is shared among four tests.
target_link_libraries (${PROJECT_NAME} mongo::mongoc_static)
