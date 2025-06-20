
#ifndef PASSENGER_H
#define PASSENGER_H

#include <iostream>
#include "../db/sqlite3.h"

class Passenger {
private:
    std::string name;
    std::string surname;
    std::string phone;
    std::string passportId;

    friend class InterfaceRender;
    friend class DatabaseManager;

public:
    Passenger() {}
    Passenger(const std::string& name,
        const std::string& surname,
        const std::string& phone,
        const std::string& passportId)
        : name(name), surname(surname), phone(phone), passportId(passportId) {
    }

    bool savePassenger(sqlite3* db) const;
};

#endif //PASSENGER_H
