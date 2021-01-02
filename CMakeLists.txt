cmake_minimum_required(VERSION 3.0)
project(autot)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wfatal-errors -Wpedantic -Wextra -Wmissing-include-dirs -Wconversion -Wsign-conversion -pthread")

file(GLOB SOURCES src/*.h src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})

add_definitions(-DCURL_STATICLIB)
message("build type: " ${CMAKE_BUILD_TYPE})

#set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)
