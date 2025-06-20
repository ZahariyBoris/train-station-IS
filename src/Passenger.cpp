#include "../headers/Passenger.h"

bool Passenger::savePassenger(sqlite3* db) const {
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