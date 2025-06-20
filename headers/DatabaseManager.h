
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <iostream>

#include "../db/sqlite3.h"
#include "InterfaceRender.h"

class DatabaseManager : public InterfaceRender {
public:
    void createTables(sqlite3* db);
    void dropTables(sqlite3* db);
    bool authenticateUser(sqlite3* db, const std::string& login, const std::string& password);
    bool addCity(sqlite3* db, const std::string& name);
    bool addStation(sqlite3* db, const std::string& name, int city_id);
    bool addTrain(sqlite3* db, const std::string& model_name, int route_id);
    bool addPlatform(sqlite3* db, const std::string& name, int station_id);
    bool deleteRoute(sqlite3* db, int id);
    bool addCarriage(sqlite3* db, int train_id, int seat_count);
    bool cancelTicket(sqlite3* db, int id);
    bool deleteExpiredTickets(sqlite3* db);
    bool getPassengerById(sqlite3* db, int passengerId, Passenger& p);
    void showAllTickets(sqlite3* db);
    void showAllPassengers(sqlite3* db);
    void showAllRoutes(sqlite3* db);
    void searchPassengerBySurname(sqlite3* db, const std::string& targetSurname);
    void searchTicketByPassengerSurname(sqlite3* db, const std::string& surname);
    bool stationExists(sqlite3* db, int station_id);
    bool platformBelongsToStation(sqlite3* db, int platform_id, int station_id);
};

#endif //DATABASEMANAGER_H
