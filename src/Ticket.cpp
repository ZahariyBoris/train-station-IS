#include "../headers/Ticket.h"

bool Ticket::saveTicket(sqlite3* db) {
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
        id = static_cast<int>(sqlite3_last_insert_rowid(db));
        std::cout << "Ticket purchased! Ticket ID: " << id << std::endl;
        success = true;
    }
    else {
        std::cerr << "Purchase error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return success;
}