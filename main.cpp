#include <iostream>
#include "sqlite3.h"

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


class DatabaseManager {
public:
    void createTables(sqlite3* db) {
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

        const char* createTablesSQL =

            // Пасажири
            "CREATE TABLE IF NOT EXISTS passengers ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "surname TEXT NOT NULL,"
                "email TEXT NOT NULL UNIQUE,"
                "phone_num TEXT NOT NULL UNIQUE,"
                "passport_id TEXT NOT NULL UNIQUE"
            ");"

            // Міста
            "CREATE TABLE IF NOT EXISTS cities ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL UNIQUE"
            ");"

            // Станції
            "CREATE TABLE IF NOT EXISTS stations ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "city_id INTEGER NOT NULL,"
                "FOREIGN KEY (city_id) REFERENCES cities(id)"
            ");"

            // Платформи
            "CREATE TABLE IF NOT EXISTS platforms ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "station_id INTEGER NOT NULL,"
                "description TEXT,"
                "FOREIGN KEY (station_id) REFERENCES stations(id)"
            ");"

            // Маршрути
            "CREATE TABLE IF NOT EXISTS routes ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "departure_time TEXT NOT NULL,"
                "arrival_time TEXT NOT NULL,"
                "departure_station_id INTEGER NOT NULL,"
                "arrival_station_id INTEGER NOT NULL,"
                "platform_id INTEGER NOT NULL,"
                "FOREIGN KEY (departure_station_id) REFERENCES stations(id),"
                "FOREIGN KEY (arrival_station_id) REFERENCES stations(id),"
                "FOREIGN KEY (platform_id) REFERENCES platforms(id)"
            ");"

            // Потяги
            "CREATE TABLE IF NOT EXISTS trains ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "model_name TEXT NOT NULL UNIQUE,"
                "route_id INTEGER NOT NULL,"
                "FOREIGN KEY (route_id) REFERENCES routes(id)"
            ");"

            // Вагони
            "CREATE TABLE IF NOT EXISTS carriages ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "train_id INTEGER NOT NULL,"
                "seats_count INTEGER NOT NULL,"
                "FOREIGN KEY (train_id) REFERENCES trains(id) ON DELETE CASCADE"
            ");"

            // Квитки
            "CREATE TABLE IF NOT EXISTS tickets ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "passenger_id INTEGER NOT NULL,"
                "train_id INTEGER NOT NULL,"
                "carriage_id INTEGER NOT NULL,"
                "seat_number INTEGER NOT NULL,"
                "price REAL NOT NULL,"
                "purchase_date TEXT NOT NULL,"
                "FOREIGN KEY (passenger_id) REFERENCES passengers(id) ON DELETE CASCADE,"
                "FOREIGN KEY (train_id) REFERENCES trains(id),"
                "FOREIGN KEY (carriage_id) REFERENCES carriages(id),"
                "UNIQUE (carriage_id, seat_number)"
            ");";

        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, createTablesSQL, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Table creation error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "All tables created successfully.\n";
        }
    }

    void dropTables(sqlite3* db) {
        const char* dropTablesSQL =
            "DROP TABLE IF EXISTS tickets;"
            "DROP TABLE IF EXISTS carriages;"
            "DROP TABLE IF EXISTS trains;"
            "DROP TABLE IF EXISTS routes;"
            "DROP TABLE IF EXISTS platforms;"
            "DROP TABLE IF EXISTS stations;"
            "DROP TABLE IF EXISTS cities;"
            "DROP TABLE IF EXISTS passengers;";

        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, dropTablesSQL, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Error dropping tables: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "All tables dropped successfully.\n";
        }
    }
};

int main() {
    sqlite3* db;
    if (sqlite3_open("railway_system.db", &db) == SQLITE_OK) {
        DatabaseManager manager;
        manager.dropTables(db);
        manager.createTables(db);
        sqlite3_close(db);
    } else {
        std::cerr << "Can't open database\n";
    }
    return 0;
}