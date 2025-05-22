#include <iostream>
#include "include/sqlite3.h"
#include "include/classes.h"

/******************************************************************************
 *                                                                            *
 *                         ГОЛОВНИЙ ФУНКЦІОНАЛ ПРОГРАМИ                       *
 *                                                                            *
 *   У цьому файлі реалізовано основну логіку моєї програми.                  *
 *                                                                            *
 *   Використані технології:                                                  *
 *     - C++20                                                                *
 *     - SQLite3                                                              *
 *     - CMake (генератор збірок)                                             *
 *     - SQLiteStudio (перегляд таблиць)                                      *
 *     - JetBrains CLion 2025.1 (середовище розробки)                         *
 *                                                                            *
 *   P.S. Програма на стадії розробки!                                        *
 *                                                                            *
 ******************************************************************************/

int main() {
    sqlite3* db;

    if (sqlite3_open("railway_system.db", &db) == SQLITE_OK) {
        sqlite3_open("railway_system.db", &db);
        DatabaseManager manager;
        // manager.dropTables(db);
        manager.createTables(db);
        // sqlite3_exec(db, "INSERT INTO passengers (name, surname, email, phone_num, passport_id) VALUES ('Ivan', 'Bogrych', 'ivanko@gmail.com', 380973126488, '9PC793764HHJ5');", nullptr, nullptr, nullptr);
        sqlite3_close(db);
    }
    else {
        std::cout << "Can't open database" << std::endl;
    }

    return 0;
}