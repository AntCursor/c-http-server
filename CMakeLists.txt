cmake_minimum_required(VERSION 3.10)
project(c-http-server)

add_executable(chttp chttp.c vector.c addrconv.c connection.c con_handler.c)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Wpedantic)
