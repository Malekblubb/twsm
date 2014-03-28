project(twsm_web)
cmake_minimum_required(VERSION 2.8)

# collect files
file(GLOB_RECURSE SRC_LIST "./include/*" "./src/*")
include_directories("./include" "../external/mlk/include" "../external/twl/include")

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} "-lwthttp -lwt")

set(CMAKE_CXX_FLAGS "-std=c++0x -O3")
set(CMAKE_CXX_COMPILER "clang++")
set(EXECUTABLE_OUTPUT_PATH "build")
