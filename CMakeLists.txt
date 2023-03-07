cmake_minimum_required(VERSION 3.24)
project(thumperd C)

set(CMAKE_C_STANDARD 11)

add_executable(thumperd src/main.c src/answer.c src/thumper_engine/thumper.c src/thumper_engine/stack.c)

target_compile_definitions(thumperd PUBLIC EMULATOR_PLEASE_TRAP) #debug remove later
target_link_libraries(thumperd microhttpd)