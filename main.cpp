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

    manager.createTables(db);
    std::cout << std::endl;
    manager.deleteExpiredTickets(db);
    std::cout << std::endl;
    sleepAndClear(999);

    char choice;
    bool isRunning = true;
    std::string role, password;

    while (isRunning) {
        clearScreen();
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
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string name;
                            if (!safeInput(name, "Enter city name: ")) break;
                            manager.addCity(db, name);
                            break;
                        }

                        case '2': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string name;
                            int city_id;
                            if (!safeStringInput(name, "Enter station name: ")) break;
                            if (!safeInput(city_id, "Enter city ID: ")) break;
                            manager.addStation(db, name, city_id);
                            break;
                        }

                        case '3': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string name;
                            int station_id;
                            if (!safeStringInput(name, "Enter platform name: ")) break;
                            if (!safeInput(station_id, "Enter station ID: ")) break;
                            manager.addPlatform(db, name, station_id);
                            break;
                        }

                        case '4': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string departure_time, arrival_time;
                            int departure_station_id, arrival_station_id, platform_id;
                            if (!safeDateTimeInput(departure_time, "Enter departure time (YYYY-MM-DD HH:MM:SS): ")) break;
                            if (!safeDateTimeInput(arrival_time, "Enter arrival time (YYYY-MM-DD HH:MM:SS): ")) break;
                            if (!safeInput(departure_station_id, "Enter departure station ID: ")) break;
                            if (!safeInput(arrival_station_id, "Enter arrival station ID: ")) break;
                            if (!safeInput(platform_id, "Enter platform ID: ")) break;
                            if (!manager.stationExists(db, departure_station_id)) {
                                std::cerr << "Departure station ID does not exist." << std::endl;
                                break;
                            }
                            if (!manager.stationExists(db, arrival_station_id)) {
                                std::cerr << "Arrival station ID does not exist." << std::endl;
                                break;
                            }
                            if (!manager.platformBelongsToStation(db, platform_id, departure_station_id)) {
                                std::cerr << "Platform does not belong to the departure station." << std::endl;
                                break;
                            }
                            Route r(departure_time, arrival_time, departure_station_id, arrival_station_id, platform_id);
                            r.saveRoute(db);
                            break;
                        }

                        case '5': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            int id;
                            if (!safeInput(id), "Enter route ID: ") break;
                            manager.deleteRoute(db, id);
                            break;
                        }

                        case '6': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "<====[ ROUTES ]===>" << std::endl;
                            manager.showAllRoutes(db);
                            pressAnyKeyToContinue();
                            clearScreen();
                            break;
                        }

                        case '7': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string model;
                            int route_id;
                            if (!safeStringInput(model, "Enter model name: ")) break;
                            if (!safeInput(route_id, "Enter route ID: ")) break;
                            manager.addTrain(db, model, route_id);
                            break;
                        }

                        case '8': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            int train_id, seat_count;
                            if (!safeInput(train_id, "Enter train ID: ")) break;
                            if (!safeInput(seat_count, "Enter seat count: ")) break;
                            manager.addCarriage(db, train_id, seat_count);
                            break;
                        }

                        case 'd': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            bool choice_del;
                            manager.dropWarning();
                            std::cin >> choice_del;
                            if (choice_del == 1) {
                                manager.dropTables(db);
                                sleepAndClear(500);
                                break;
                            }
                            else {
                                std::cout << "Table deleting has been canceled!" << std::endl;
                                sleepAndClear(500);
                                break;
                            }
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
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string name, surname, passport_id, phone_num;
                            if (!safeStringInput(name, "Enter passenger's name: ")) break;
                            if (!safeStringInput(surname, "Enter passenger's surname: ")) break;
                            if (!safeStringInput(passport_id, "Enter passenger's passport ID: ")) break;
                            if (!safeStringInput(phone_num, "Enter passenger's phone number: ")) break;
                            Passenger p(name, surname, phone_num, passport_id);
                            p.savePassenger(db);
                            break;
                        }

                        case '2': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            int p_id, tr_id, carriage_id, seat;
                            double price;
                            std::string date = getCurrentDateTime();

                            if (!safeInput(p_id, "Enter passenger's ID: ")) break;
                            if (!safeInput(tr_id, "Enter train ID: ")) break;
                            if (!safeInput(carriage_id, "Enter carriage ID: ")) break;
                            if (!safeInput(seat, "Enter seat ID: ")) break;
                            if (!safeInput(price, "Enter ticket price: ")) break;

                            Ticket t(p_id, tr_id, carriage_id, seat, price, date);
                            t.saveTicket(db);
                            break;
                        }

                        case '3': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            int p_id;
                            if (!safeInput(p_id, "Enter passenger's ID: ")) break;
                            manager.cancelTicket(db, p_id);
                            break;
                        }

                        case '4': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "<====[ PASSENGERS ]===>" << std::endl;
                            manager.showAllPassengers(db);
                            pressAnyKeyToContinue();
                            clearScreen();
                            break;
                        }

                        case '5': {
                            clearScreen();
                            std::cout << "<====[ TICKETS ]===>" << std::endl;
                            manager.showAllTickets(db);
                            pressAnyKeyToContinue();
                            clearScreen();
                            break;
                        }

                        case '6': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "<====[ ROUTES ]===>" << std::endl;
                            manager.showAllRoutes(db);
                            pressAnyKeyToContinue();
                            clearScreen();
                            break;
                        }

                        case '7': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string surname;
                            if (!safeStringInput(surname, "Enter passenger's surname: ")) break;
                            manager.searchPassengerBySurname(db, surname);
                            pressAnyKeyToContinue();
                            clearScreen();
                            break;
                        }

                        case '8': {
                            clearScreen();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::string surname;
                            if (!safeStringInput(surname, "Enter passenger's surname: ")) break;
                            manager.searchTicketByPassengerSurname(db, surname);
                            pressAnyKeyToContinue();
                            clearScreen();
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
                sleepAndClear(700);
            }
            break;

        case 'q':
            sleepAndClear(500);
            std::cout << "Quitting..." << std::endl;
            sleepAndClear(500);
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