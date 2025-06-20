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

void DatabaseManager::dropTables(sqlite3* db) {
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
    }
    else {
        std::cout << "All tables dropped successfully.\n";
    }
}

bool DatabaseManager::authenticateUser(sqlite3* db, const std::string& login, const std::string& password) {
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

bool DatabaseManager::addCity(sqlite3* db, const std::string& name) {
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

bool DatabaseManager::addStation(sqlite3* db, const std::string& name, int city_id) {
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
    }
    else {
        int errCode = sqlite3_errcode(db);
        if (errCode == SQLITE_CONSTRAINT) {
            std::cerr << "Station already exists or city_id invalid." << std::endl;
        }
        else {
            std::cerr << "Error inserting station: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::addTrain(sqlite3* db, const std::string& model_name, int route_id) {
    const char* sql = "INSERT INTO trains (model_name, route_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, model_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, route_id);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Train added successfully!" << std::endl;
        success = true;
    }
    else {
        std::cerr << "Error inserting train: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::addPlatform(sqlite3* db, const std::string& name, int station_id) {
    const char* sql = "INSERT INTO platforms (name, station_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, station_id);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Platform added successfully!" << std::endl;
        success = true;
    }
    else {
        std::cerr << "Error inserting platform: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::deleteRoute(sqlite3* db, int id) {
    const char* sql = "DELETE FROM route WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
        std::cerr << "Failed to bind ID: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to delete route: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "Route deleted successfully.\n";
    return true;
}

bool DatabaseManager::addCarriage(sqlite3* db, int train_id, int seat_count) {
    const char* sql = "INSERT INTO carriages (train_id, seats_count) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, train_id);
    sqlite3_bind_int(stmt, 2, seat_count);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Carriage added successfully!" << std::endl;
        success = true;
    }
    else {
        std::cerr << "Error inserting carriage: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::cancelTicket(sqlite3* db, int id) {
    const char* sql = "DELETE FROM tickets WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Ticket canceled successfully!" << std::endl;
        success = true;
    } else {
        std::cerr << "Error cancelling ticket: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::deleteExpiredTickets(sqlite3* db) {
    const char* sql = "DELETE FROM tickets WHERE train_id IN (SELECT id FROM routes WHERE departure_time < datetime('now'));";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error deleting expired tickets: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Expired tickets deleted successfully.\n";
    return true;
}

bool DatabaseManager::getPassengerById(sqlite3* db, int passengerId, Passenger& p) {
    const char* sql = "SELECT * FROM passengers WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, passengerId);

    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        p.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        p.surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        found = true;
    }

    sqlite3_finalize(stmt);
    return found;
}

void DatabaseManager::showAllTickets(sqlite3* db) {
    const char* sql = "SELECT * FROM tickets";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Ticket t;
            t.id = sqlite3_column_int(stmt, 0);
            t.passenger_id = sqlite3_column_int(stmt, 1);
            t.train_id = sqlite3_column_int(stmt, 2);
            t.carriage_id = sqlite3_column_int(stmt, 3);
            t.seat_number = sqlite3_column_int(stmt, 4);
            t.price = sqlite3_column_double(stmt, 5);
            t.purchase_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

            Passenger p;
            if (getPassengerById(db, t.passenger_id, p)) {
                InterfaceRender renderer;
                renderer.printTicket(t, p);
            } else {
                std::cerr << "Passenger with ID " << t.passenger_id << " not found.\n";
            }
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to retrieve tickets: " << sqlite3_errmsg(db) << "\n";
    }
}

void DatabaseManager::showAllPassengers(sqlite3* db) {
    const char* sql = "SELECT id, name, surname, phone_num, passport_id FROM passengers;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare SELECT statement: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    InterfaceRender render;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Passenger p(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)), // name
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), // surname
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)), // phone
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))  // passport_id
        );
        Ticket t;
        t.passenger_id = sqlite3_column_int(stmt, 0); // id
        render.printPassenger(p, t);
    }

    sqlite3_finalize(stmt);
}

void DatabaseManager::showAllRoutes(sqlite3* db) {
    const char* sql = R"(
        SELECT
            r.departure_time,
            r.arrival_time,
            s1.name AS departure_station_name,
            s2.name AS arrival_station_name,
            p.name AS platform_name
        FROM routes r
        JOIN stations s1 ON r.departure_station_id = s1.id
        JOIN stations s2 ON r.arrival_station_id = s2.id
        JOIN platforms p ON r.platform_id = p.id;
    )";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Route r;
            r.departure_t = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            r.arrival_t = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            r.departure_station_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            r.arrival_station_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            r.platform_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            InterfaceRender renderer;
            renderer.printRoute(r);
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Unable to retrieve routes: " << sqlite3_errmsg(db) << "\n";
    }
}

void DatabaseManager::searchPassengerBySurname(sqlite3* db, const std::string& targetSurname) {
    const char* sql = "SELECT id, name, surname, phone_num, passport_id FROM passengers WHERE surname = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, targetSurname.c_str(), -1, SQLITE_TRANSIENT);

        bool found = false;
        InterfaceRender render;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Passenger p(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)), // name
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), // surname
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)), // phone
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))  // passport_id
            );
            Ticket t;
            t.passenger_id = sqlite3_column_int(stmt, 0); // id
            render.printPassenger(p, t);
            found = true;
        }

        if (!found) {
            std::cout << "No passengers found with surname: " << targetSurname << "\n";
        }

        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Search failed: " << sqlite3_errmsg(db) << "\n";
    }
}

void DatabaseManager::searchTicketByPassengerSurname(sqlite3* db, const std::string& surname) {
    const char* sql =
        "SELECT t.passenger_id, t.train_id, t.carriage_id, t.seat_number, t.price, t.purchase_date, "
        "p.name, p.surname "
        "FROM tickets t "
        "JOIN passengers p ON t.passenger_id = p.id "
        "WHERE p.surname = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, surname.c_str(), -1, SQLITE_TRANSIENT);

        bool found = false;
        InterfaceRender render;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Ticket t;
            Passenger p;

            t.passenger_id = sqlite3_column_int(stmt, 0);
            t.train_id = sqlite3_column_int(stmt, 1);
            t.carriage_id = sqlite3_column_int(stmt, 2);
            t.seat_number = sqlite3_column_int(stmt, 3);
            t.price = sqlite3_column_double(stmt, 4);
            t.purchase_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

            p.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            p.surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

            render.printTicket(t, p);
            found = true;
        }

        if (!found) {
            std::cout << "No tickets found for passenger with surname: " << surname << "\n";
        }

        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to search tickets: " << sqlite3_errmsg(db) << "\n";
    }
}

bool DatabaseManager::stationExists(sqlite3* db, int station_id) {
    const char* sql = "SELECT COUNT(*) FROM stations WHERE id = ?";
    sqlite3_stmt* stmt;
    bool exists = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, station_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
    }
    return exists;
}

bool DatabaseManager::platformBelongsToStation(sqlite3* db, int platform_id, int station_id) {
    const char* sql = "SELECT COUNT(*) FROM platforms WHERE id = ? AND station_id = ?";
    sqlite3_stmt* stmt;
    bool valid = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, platform_id);
        sqlite3_bind_int(stmt, 2, station_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            valid = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
    }
    return valid;
}