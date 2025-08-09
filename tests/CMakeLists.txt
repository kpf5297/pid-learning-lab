cmake_minimum_required(VERSION 3.10)
project(pid_tests C)

set(CMAKE_C_STANDARD 11)

add_executable(pid_test pid_test.c ../03-pi-control/Core/Src/pid.c)
target_include_directories(pid_test PRIVATE ../03-pi-control/Core/Inc)
target_link_libraries(pid_test m)

enable_testing()
add_test(NAME pid_test COMMAND pid_test)
