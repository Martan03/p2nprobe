cmake_minimum_required(VERSION 3.16)

project(p2nprobe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(p2nprobe)

target_link_libraries(p2nprobe PRIVATE
    pcap
)

add_subdirectory(src)
