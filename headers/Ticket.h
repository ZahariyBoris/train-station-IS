
#ifndef TICKET_H
#define TICKET_H

#include <iostream>
#include "../db/sqlite3.h"

class Ticket {
private:
    int id;
    int passenger_id;
    int train_id;
    int carriage_id;
    int seat_number;
    double price;
    std::string purchase_date;

    friend class InterfaceRender;
    friend class DatabaseManager;

public:
    Ticket() {}
    Ticket(const int& p_id,
        const int& t_id,
        const int& c_id,
        const int& s,
        const double& p,
        const std::string& d)
        : passenger_id(p_id), train_id(t_id), carriage_id(c_id), seat_number(s), price(p), purchase_date(d) {
    }

    bool saveTicket(sqlite3* db);
};

#endif //TICKET_H
