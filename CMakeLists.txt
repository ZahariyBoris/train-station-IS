cmake_minimum_required(VERSION 3.31)
project(Kursova)

set(CMAKE_CXX_STANDARD 20)

add_executable (
        Kursova main.cpp
        db/sqlite3.c
        headers/functions.h
        src/Ticket.cpp
        headers/Ticket.h
        src/Passenger.cpp
        headers/Passenger.h
        src/Route.cpp
        headers/Route.h
        headers/InterfaceRender.h
        src/DatabaseManager.cpp
        headers/DatabaseManager.h
)