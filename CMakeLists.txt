cmake_minimum_required(VERSION 3.31)
project(Kursova)

set(CMAKE_CXX_STANDARD 20)

add_executable (
        Kursova main.cpp
        sqlite3.c
)