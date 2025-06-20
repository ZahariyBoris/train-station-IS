
#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include "../db/sqlite3.h"

class Route {
private:
    std::string departure_t;
    std::string arrival_t;
    int departure_st;
    int arrival_st;
    int platform;

    std::string departure_station_name;
    std::string arrival_station_name;
    std::string platform_name;

    friend class InterfaceRender;
    friend class DatabaseManager;;

public:
    Route() {}
    Route(const std::string& d_t,
        const std::string& a_t,
        const int& d_st,
        const int& a_st,
        const int& p)
        : departure_t(d_t), arrival_t(a_t), departure_st(d_st), arrival_st(a_st), platform(p) {
    }

    bool saveRoute(sqlite3* db) const;
};

#endif //ROUTE_H
