//
// Created by zahar on 20.06.2025.
//

#include "../headers/DatabaseManager.h"

void DatabaseManager::createTables(sqlite3* db) {
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    const char* createTablesSQL =

        "CREATE TABLE IF NOT EXISTS roles ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL"
        ");"

        "CREATE TABLE IF NOT EXISTS passengers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "surname TEXT NOT NULL,"
        "email TEXT UNIQUE,"
        "phone_num TEXT NOT NULL UNIQUE,"
        "passport_id TEXT NOT NULL UNIQUE"
        ");"

        "CREATE TABLE IF NOT EXISTS cities ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL UNIQUE"
        ");"

        "CREATE TABLE IF NOT EXISTS stations ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "city_id INTEGER NOT NULL,"
        "FOREIGN KEY (city_id) REFERENCES cities(id)"
        ");"

        "CREATE TABLE IF NOT EXISTS platforms ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "station_id INTEGER NOT NULL,"
        "description TEXT,"
        "FOREIGN KEY (station_id) REFERENCES stations(id)"
        ");"

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

        "CREATE TABLE IF NOT EXISTS trains ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "model_name TEXT NOT NULL UNIQUE,"
        "route_id INTEGER NOT NULL,"
        "FOREIGN KEY (route_id) REFERENCES routes(id)"
        ");"

        "CREATE TABLE IF NOT EXISTS carriages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "train_id INTEGER NOT NULL,"
        "seats_count INTEGER NOT NULL,"
        "FOREIGN KEY (train_id) REFERENCES trains(id) ON DELETE CASCADE"
        ");"

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
    }
    else {
        std::cout << "All tables created successfully.\n";
    }
}