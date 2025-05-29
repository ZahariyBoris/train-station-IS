#include <iostream>
#include <chrono>
#include <string>
#include <limits>
#include "include/sqlite3.h"
#include "include/classes.h"
#include "include/functions.h"

/******************************************************************************
 *                                                                            *
 *                         ГОЛОВНИЙ ФУНКЦІОНАЛ ПРОГРАМИ                       *
 *                                                                            *
 *   У цьому файлі реалізовано основну логіку моєї програми.                  *
 *                                                                            *
 *   Використані технології:                                                  *
 *     - C++20                                                                *
 *     - SQLite3                                                              *
 *     - CMake (генератор збірок)                                             *
 *     - SQLiteStudio (перегляд таблиць)                                      *
 *     - JetBrains CLion 2025.1 (середовище розробки)                         *
 *                                                                            *
 *   P.S. Програма на стадії розробки!                                        *
 *                                                                            *
 ******************************************************************************/

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
                            std::string name, surname, passport_id, phone_num;
                            std::cout << "Enter passenger's name: ";
                            std::cin >> name;
                            std::cout << "Enter passenger's surname: ";
                            std::cin >> surname;
                            std::cout << "Enter passenger's passport ID: ";
                            std::cin >> passport_id;
                            std::cout << "Enter passenger's phone number: ";
                            std::cin >> phone_num;
                            manager.addPassenger(db, name, surname, phone_num, passport_id);
                            break;
                        }

                        case '2': {
                            clearScreen();
                            std::string name;
                            std::cout << "Enter city name: ";
                            std::cin >> name;
                            manager.addCity(db, name);
                            break;
                        }

                        case '3': {
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
                            manager.addPassenger(db, name, surname, phone_num, passport_id);
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