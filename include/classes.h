
#ifndef CLASSES_H
#define CLASSES_H

#include "sqlite3.h"

class Ticket {
public:
    int passenger_id;
    int train_id;
    int carriage_id;
    int seat_number;
    double price;
    std::string purchase_date;

    Ticket() {}
    Ticket(const int& p_id,
        const int& t_id,
        const int& c_id,
        const int& s,
        const double& p,
        const std::string& d)
        : passenger_id(p_id), train_id(t_id), carriage_id(c_id), seat_number(s), price(p), purchase_date(d) {
    }

    bool saveTicket(sqlite3* db) const {
        const char* sql = "INSERT INTO tickets (passenger_id, train_id, carriage_id, seat_number, price, purchase_date) VALUES (?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        const char* seatCheckSql = "SELECT seats_count FROM carriages WHERE id = ?";
        sqlite3_stmt* seatStmt;

        if (sqlite3_prepare_v2(db, seatCheckSql, -1, &seatStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(seatStmt, 1, carriage_id);
            if (sqlite3_step(seatStmt) == SQLITE_ROW) {
                int maxSeats = sqlite3_column_int(seatStmt, 0);
                if (seat_number < 1 || seat_number > maxSeats) {
                    std::cerr << "Invalid seat number! This carriage has only " << maxSeats << " seats." << std::endl;
                    sqlite3_finalize(seatStmt);
                    return false;
                }
            }
        }
        sqlite3_finalize(seatStmt);

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, passenger_id);
        sqlite3_bind_int(stmt, 2, train_id);
        sqlite3_bind_int(stmt, 3, carriage_id);
        sqlite3_bind_int(stmt, 4, seat_number);
        sqlite3_bind_double(stmt, 5, price);
        sqlite3_bind_text(stmt, 6, purchase_date.c_str(), -1, SQLITE_TRANSIENT);

        bool success = false;
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Ticket purchased!" << std::endl;
            success = true;
        }
        else {
            std::cerr << "Purchase error: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return success;
    }
};

class Passenger {
public:
    std::string name;
    std::string surname;
    std::string phone;
    std::string passportId;

    Passenger() {}
    Passenger(const std::string& name,
        const std::string& surname,
        const std::string& phone,
        const std::string& passportId)
        : name(name), surname(surname), phone(phone), passportId(passportId) {
    }

    bool savePassenger(sqlite3* db) const {
        sqlite3_stmt* stmt = nullptr;
        const char* sql = "INSERT INTO passengers (name, surname, phone_num, passport_id) VALUES (?, ?, ?, ?);";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, phone.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, passportId.c_str(), -1, SQLITE_TRANSIENT);

        bool success = sqlite3_step(stmt) == SQLITE_DONE;
        if (!success) {
            std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
        }
        else {
            std::cout << "Passenger saved.\n";
        }

        sqlite3_finalize(stmt);
        return success;
    }
};

class InterfaceRender {
public:
    void drawLoginMenu() {
        std::cout << "-------------------------" << std::endl;
        std::cout << "| WELCOME TO THE SYSTEM |" << std::endl;
        std::cout << "------------------------- \n"
                  << "| 1 - Login \n"
                  << "| q - QUIT \n"
                  << "| Enter your choice from the menu: ";
    }

    void drawAdminMenu() {
        std::cout << "--------------------------" << std::endl;
        std::cout << "| Choose action:" << std::endl;
        std::cout << "| 1 - Add city;" << std::endl;
        std::cout << "| 2 - Add station;" << std::endl;
        std::cout << "| 3 - Add platform;" << std::endl;
        std::cout << "| 4 - Add route;" << std::endl;
        std::cout << "| 5 - Add train" << std::endl;
        std::cout << "| 6 - Add carriage" << std::endl;
        std::cout << "|" << std::endl;
        std::cout << "| q - QUIT" << std::endl;
        std::cout << "--------------------------" << std::endl;
        std::cout << "Enter your choice from the menu: ";
    }

    void drawCashierMenu() {
        std::cout << "--------------------------" << std::endl;
        std::cout << "| Choose action:" << std::endl;
        std::cout << "| 1 - Add passenger;" << std::endl;
        std::cout << "| 2 - Buy ticket;" << std::endl;
        std::cout << "| 3 - Cancel ticket" << std::endl;
        std::cout << "|" << std::endl;
        std::cout << "| 4 - Show all passengers;" << std::endl;
        std::cout << "| 5 - Show all tickets;" << std::endl;
        std::cout << "| q - QUIT" << std::endl;
        std::cout << "--------------------------" << std::endl;
        std::cout << "Enter your choice from the menu: ";
    }

    void printPassenger(const Passenger& p) const {
        std::cout << "-----------------------------------------------------------" << std::endl;
        std::cout << "| Passenger name: " << p.name << std::endl;
        std::cout << "| Passenger surname: " << p.surname << std::endl;
        std::cout << "| Passenger phone number: " << p.phone << std::endl;
        std::cout << "| Passenger passport ID: " << p.passportId << std::endl;
        std::cout << "-----------------------------------------------------------" << std::endl;
    }

    void printTicket(const Ticket& t, const Passenger& p) const {
        std::cout << "-----------------------------------------------------------" << std::endl;
        std::cout << "| Passenger ID: " << t.passenger_id << std::endl;
        std::cout << "| Passenger name: " << p.name << std::endl;
        std::cout << "| Passenger surname: " << p.surname << std::endl;
        std::cout << "| Train ID: " << t.train_id << std::endl;
        std::cout << "| Carriage ID: " << t.carriage_id << std::endl;
        std::cout << "| Seat number: " << t.seat_number << std::endl;
        std::cout << "| Ticket price: " << t.price << std::endl;
        std::cout << "| Purchase date: " << t.purchase_date << std::endl;
        std::cout << "---------------------------------------------------------------" << std::endl;
    }
};

class DatabaseManager : public InterfaceRender {
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
        }
        else {
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
        }
        else {
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

    bool addTrain(sqlite3* db, const std::string& model_name, int route_id) {
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

    bool addPlatform(sqlite3* db, const std::string& name, int station_id) {
        const char* sql = "INSERT INTO trains (name, station_id) VALUES (?, ?);";
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

    bool addRoute(sqlite3* db,
        const std::string& departure_time,
        const std::string& arrival_time,
        int departure_station_id,
        int arrival_station_id,
        int platform_id) {
        const char* sql = "INSERT INTO routes (departure_time, arrival_time, departure_station_id, arrival_station_id, platform_id) "
            "VALUES (?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, departure_time.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, arrival_time.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, departure_station_id);
        sqlite3_bind_int(stmt, 4, arrival_station_id);
        sqlite3_bind_int(stmt, 5, platform_id);

        bool success = false;
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Route added successfully!" << std::endl;
            success = true;
        }
        else {
            std::cerr << "Error inserting route: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return success;
    }

    bool addCarriage(sqlite3* db, int train_id, int seat_count) {
        const char* sql = "INSERT INTO carriages (train_id, seat_count) VALUES (?, ?);";
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

    bool cancelTicket(sqlite3* db, int p_id) {
        const char* sql = "DELETE FROM tickets WHERE passenger_id = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, p_id);

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

    bool deleteExpiredTickets(sqlite3* db) {
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

    bool getPassengerById(sqlite3* db, int passengerId, Passenger& p) {
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

    void showAllTickets(sqlite3* db) {
        const char* sql = "SELECT * FROM tickets";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Ticket t;
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
        } else {
            std::cerr << "Failed to retrieve tickets: " << sqlite3_errmsg(db) << "\n";
        }
    }

    void showAllPassengers(sqlite3* db) {
        const char* sql = "SELECT name, surname, phone_num, passport_id FROM passengers;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare SELECT statement: " << sqlite3_errmsg(db) << "\n";
            return;
        }

        InterfaceRender render;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Passenger p(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
            );
            render.printPassenger(p);
        }

        sqlite3_finalize(stmt);
    }
};

#endif //CLASSES_H