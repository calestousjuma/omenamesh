cmake_minimum_required(VERSION 3.14)
project(CurlTest C)

set(CMAKE_C_STANDARD 99)

set(CURL_LIBRARY "-lcurl")
find_package(CURL)
add_executable(CurlTest main.c)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(CurlTest ${CURL_LIBRARIES})
