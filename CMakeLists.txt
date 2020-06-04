cmake_minimum_required(VERSION 3.16)
project(CoroutinesSample)

set(CMAKE_CXX_STANDARD 20)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    add_definitions(/await)
endif ()

add_executable(co_return co_return.cpp)
add_executable(co_yield co_yield.cpp)