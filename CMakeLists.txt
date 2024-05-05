cmake_minimum_required(VERSION 3.26)
project(osi_hw_10 C)

set(CMAKE_C_STANDARD 23)

add_executable(osi_hw_10 main.c
        server.c
        client1.c
        client2.c)
