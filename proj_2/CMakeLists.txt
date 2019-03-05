cmake_minimum_required(VERSION 3.13)
project(xt1_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(xt1_2
        create.c
        event.c
        main.c
        Makefile
        proc.h
        resched.c
        xmain.c
        yield.c)
