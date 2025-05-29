
#ifndef CLASSES_H
#define CLASSES_H

#include "sqlite3.h"

class IntefaceRender {
public:
    void drawLoginMenu() {
        std::cout << "-------------------------" << std::endl;
        std::cout << "| WELCOME TO THE SYSTEM |" << std::endl;
        std::cout << "------------------------- \n"
                  << "1 - Login \n"
                  << "q - Exit \n"
                  << "Enter your choice from the menu: ";
    }

    void drawAdminMenu() {
        std::cout << "--------------------------" << std::endl;
        std::cout << "Choose action:" << std::endl;
        std::cout << "1 - Add passenger;" << std::endl;
        std::cout << "2 - Add city;" << std::endl;
        std::cout << "3 - Add station;" << std::endl;
        std::cout << "4 - " << std::endl;
        std::cout << "5 - " << std::endl;
        std::cout << "6 - " << std::endl;
        std::cout << "q - " << std::endl;
        std::cout << "--------------------------" << std::endl;
        std::cout << "Enter your choice from the menu: ";
    }

    void drawCashierMenu() {
        std::cout << "--------------------------" << std::endl;
        std::cout << "Choose action:" << std::endl;
        std::cout << "1 - Add passenger;" << std::endl;
        std::cout << "2 - " << std::endl;
        std::cout << "3 - " << std::endl;
        std::cout << "4 - " << std::endl;
        std::cout << "q - " << std::endl;
        std::cout << "--------------------------" << std::endl;
        std::cout << "Enter your choice from the menu: ";
    }
};

class DatabaseManager : public IntefaceRender {
public:
    void createTables(sqlite3* db) {
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

    bool authenticateUser(sqlite3* db, const std::string& login, const std::string& password) {
        const char* sql = "SELECT COUNT(*) FROM roles WHERE name = ? AND password = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        bool authenticated = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            authenticated = (count > 0);
        }

        sqlite3_finalize(stmt);
        return authenticated;
    }

    bool addPassenger(sqlite3* db, const std::string& name, const std::string& surname, const std::string& phone_num,
                  const std::string& passport_id) {
        const char* sql = "INSERT INTO passengers (name, surname, phone_num, passport_id) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, phone_num.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, passport_id.c_str(), -1, SQLITE_TRANSIENT);

        bool success = false;
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Passenger added successfully!" << std::endl;
            success = true;
        }
        else {
            int errCode = sqlite3_errcode(db);
            if (errCode == SQLITE_CONSTRAINT) {
                std::cerr << "Passenger with this passport ID already exists." << std::endl;
            } else {
                std::cerr << "Error inserting passenger: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        sqlite3_finalize(stmt);
        return success;
    }

    bool addCity(sqlite3* db, const std::string& name) {
        const char* sql = "INSERT INTO cities (name) VALUES (?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

        bool success = false;
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "City added successfully!" << std::endl;
            success = true;
        }
        else {
            int errCode = sqlite3_errcode(db);
            if (errCode == SQLITE_CONSTRAINT) {
                std::cerr << "City with this city ID already exists." << std::endl;
            }
            else {
                std::cerr << "Error inserting city: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        sqlite3_finalize(stmt);
        return success;
    }

    bool addStation(sqlite3* db, const std::string& name, int city_id) {
        const char* sql = "INSERT INTO stations (name, city_id) VALUES (?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, city_id);

        bool success = false;
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Station added successfully!" << std::endl;
            success = true;
        } else {
            int errCode = sqlite3_errcode(db);
            if (errCode == SQLITE_CONSTRAINT) {
                std::cerr << "Station already exists or city_id invalid." << std::endl;
            } else {
                std::cerr << "Error inserting station: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        sqlite3_finalize(stmt);
        return success;
    }
};

#endif //CLASSES_H