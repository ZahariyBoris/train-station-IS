#include <iostream>
#include <chrono>
#include <string>
#include <limits>
#include "include/sqlite3.h"
#include "include/classes.h"
#include "include/functions.h"

int main() {
    enableANSI();

    sqlite3* db;
    DatabaseManager manager;
    if (sqlite3_open("railway_system.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    // manager.dropTables(db);
    manager.createTables(db);
    sleepAndClear(999);

    char choice;
    bool isRunning = true;
    std::string role, password;

    while (isRunning) {
        manager.drawLoginMenu();
        std::cin >> choice;

        switch (choice) {
        case '1':
            clearScreen();
            std::cout << "Enter role: ";
            std::cin >> role;
            std::cout << "Enter password: ";
            std::cin >> password;

            if (manager.authenticateUser(db, role, password)) {
                std::cout << "Login successful!" << std::endl;
                sleepAndClear(700);

                if (role == "admin") {
                    bool inAdminMenu = true;
                    while (inAdminMenu) {
                        manager.drawAdminMenu();
                        char adminChoice;
                        std::cin >> adminChoice;

                        switch (adminChoice) {
                            case '1': {
                            clearScreen();
                            std::string name;
                            std::cout << "Enter city name: ";
                            std::cin >> name;
                            manager.addCity(db, name);
                            break;
                        }

                        case '2': {
                            clearScreen();
                            std::string name;
                            int city_id;
                            std::cout << "Enter station name: ";
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::getline(std::cin, name);
                            std::cout << "Enter city ID: ";
                            std::cin >> city_id;
                            manager.addStation(db, name, city_id);
                            break;
                        }

                        case '3': {
                            clearScreen();
                            std::string name;
                            int station_id;
                            std::cout << "Enter platform name: ";
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::getline(std::cin, name);
                            std::cout << "Enter station ID: ";
                            std::cin >> station_id;
                            manager.addPlatform(db, name, station_id);
                            break;
                        }

                        case '4': {
                            clearScreen();
                            std::string departure_time, arrival_time;
                            int departure_station_id, arrival_station_id, platform_id;
                            std::cout << "Enter departure time: ";
                            std::cin >> departure_time;
                            std::cout << "Enter arrival time: ";
                            std::cin >> arrival_time;
                            std::cout << "Enter departure station ID: ";
                            std::cin >> departure_station_id;
                            std::cout << "Enter arrival station ID: ";
                            std::cin >> arrival_station_id;
                            std::cout << "Enter platform ID: ";
                            std::cin >> platform_id;
                            manager.addRoute(db, departure_time, arrival_time, departure_station_id, arrival_station_id, platform_id);
                            break;
                        }

                        case '5': {
                            clearScreen();
                            std::string model;
                            int route_id;
                            std::cout << "Enter model name: ";
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::getline(std::cin, model);
                            std::cout << "Enter route ID: ";
                            std::cin >> route_id;
                            manager.addTrain(db, model, route_id);
                            break;
                        }

                        case '6': {
                            int train_id, seat_count;
                            std::cout << "Enter train ID: ";
                            std::cin >> train_id;
                            std::cout << "Enter seat count: ";
                            std::cin >> seat_count;
                            manager.addCarriage(db, train_id, seat_count);
                            break;
                        }

                        case 'q':
                            inAdminMenu = false;
                            break;

                        default:
                            std::cout << "Invalid choice. Try again." << std::endl;
                            break;
                        }
                    }
                }
                else if (role == "cashier") {
                    bool inCashierMenu = true;
                    while (inCashierMenu) {
                        manager.drawCashierMenu();
                        char cashierChoice;
                        std::cin >> cashierChoice;

                        switch (cashierChoice) {
                        case '1': {
                            clearScreen();
                            std::string name, surname, passport_id, phone_num;
                            std::cout << "Enter passenger's name: ";
                            std::cin >> name;
                            std::cout << "Enter passenger's surname: ";
                            std::cin >> surname;
                            std::cout << "Enter passenger's passport ID: ";
                            std::cin >> passport_id;
                            std::cout << "Enter passenger's phone number: ";
                            std::cin >> phone_num;
                            Passenger p(name, surname, phone_num, passport_id);
                            p.save(db);
                            break;
                        }

                        case '2': {
                            clearScreen();
                            int p_id, tr_id, carriage_id, seat;
                            double price;
                            std::string date = getCurrentDateTime();

                            std::cout << "Enter passenger's ID: ";
                            std::cin >> p_id;
                            std::cout << "Enter train ID: ";
                            std::cin >> tr_id;
                            std::cout << "Enter carriage ID: ";
                            std::cin >> carriage_id;
                            std::cout << "Enter seat ID: ";
                            std::cin >> seat;
                            std::cout << "Enter ticket price: ";
                            manager.purchaseTicket(db, p_id, tr_id, carriage_id, seat, price, date);
                            break;
                        }

                        case 'q':
                            inCashierMenu = false;
                            break;

                        default:
                            std::cout << "Invalid choice or not implemented yet." << std::endl;
                            break;
                        }
                    }
                }
            }
            else {
                std::cout << "Login failed! Try again!" << std::endl;
            }
            break;

        case 'q':
            sleepAndClear(500);
            std::cout << "Quitting..." << std::endl;
            isRunning = false;
            break;

        default:
            std::cout << "Invalid choice. Try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    sqlite3_close(db);

    return 0;
}