cmake_minimum_required(VERSION 3.20)
project(NumericMethods1)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_STANDARD 20)

set(CONAN_DISABLE_CHECK_COMPILER TRUE)
include(libraries/conan_paths.cmake)

find_package(Boost REQUIRED)

add_executable(NumericMethods1 main.cpp Resolver.h Utils.h Utils.cpp Resolver.cpp)
target_link_libraries(${PROJECT_NAME} Boost::headers)
