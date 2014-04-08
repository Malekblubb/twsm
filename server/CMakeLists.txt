project(twsms)
cmake_minimum_required(VERSION 2.8)

# collect files
file(GLOB_RECURSE SRC_LIST "./include/*" "./*")
include_directories("./include" "../external/mlk/include")

add_executable(${PROJECT_NAME} ${SRC_LIST})

# set compiler flags
set(CMAKE_CXX_FLAGS "-std=c++1y -stdlib=libc++ -O3")
set(CMAKE_CXX_COMPILER "clang++")
