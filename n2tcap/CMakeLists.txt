cmake_minimum_required(VERSION 3.16)

project(n2tcap VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(n2tcap)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(n2tcap PRIVATE -Wall -Wextra -Werror -fsanitize=address)
    target_link_libraries(n2tcap PRIVATE -fsanitize=address)
endif()

add_subdirectory(src)

target_include_directories(n2tcap PRIVATE ../src)

target_sources(n2tcap PRIVATE
    ../src/netflow/netflow.hpp
    ../src/netflow/header.hpp
)
