#include "../headers/Route.h"

bool Route::saveRoute(sqlite3* db) const {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO routes (departure_time, arrival_time, departure_station_id, arrival_station_id, platform_id) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, departure_t.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, arrival_t.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, departure_st);
    sqlite3_bind_int(stmt, 4, arrival_st);
    sqlite3_bind_int(stmt, 5, platform);

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