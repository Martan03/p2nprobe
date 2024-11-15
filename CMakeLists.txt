cmake_minimum_required(VERSION 3.16)

project(p2nprobe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(p2nprobe)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fsanitize=address")
    target_link_libraries(p2nprobe PRIVATE -fsanitize=address)
endif()

target_link_libraries(p2nprobe PRIVATE
    pcap
)

add_subdirectory(src)
