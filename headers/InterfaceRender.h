
#ifndef INTERFACERENDER_H
#define INTERFACERENDER_H

#include <iostream>

#include "Passenger.h"
#include "Ticket.h"
#include "Route.h"

class InterfaceRender {
public:
    void drawLoginMenu() {
        std::cout << "+-----------------------+" << std::endl;
        std::cout << "| WELCOME TO THE SYSTEM |" << std::endl;
        std::cout << "+-----------------------+" << std::endl
                  << "| 1 - Login;" << std::endl
                  << "| q - QUIT;" << std::endl
                  << "| Enter your choice from the menu: ";
    }

    void drawAdminMenu() {
        std::cout << "+------------------------+" << std::endl;
        std::cout << "| Choose action:" << std::endl;
        std::cout << "| 1 - Add city;" << std::endl;
        std::cout << "| 2 - Add station;" << std::endl;
        std::cout << "| 3 - Add platform;" << std::endl;
        std::cout << std::endl;
        std::cout << "| 4 - Add route;" << std::endl;
        std::cout << "| 5 - Delete route;" << std::endl;
        std::cout << "| 6 - Show all routes;" << std::endl;
        std::cout << std::endl;
        std::cout << "| 7 - Add train;" << std::endl;
        std::cout << "| 8 - Add carriage;" << std::endl;
        std::cout << "|" << std::endl;
        std::cout << "| d - DELETE ALL DATA;" << std::endl;
        std::cout << "| q - QUIT;" << std::endl;
        std::cout << "+------------------------+" << std::endl;
        std::cout << "Enter your choice from the menu: ";
    }

    void drawCashierMenu() {
        std::cout << "+------------------------+" << std::endl;
        std::cout << "| Choose action:" << std::endl;
        std::cout << "| 1 - Add passenger;" << std::endl;
        std::cout << "| 2 - Buy ticket;" << std::endl;
        std::cout << "| 3 - Cancel ticket;" << std::endl;
        std::cout << "|" << std::endl;
        std::cout << "| 4 - Show all passengers;" << std::endl;
        std::cout << "| 5 - Show all tickets;" << std::endl;
        std::cout << "| 6 - Show all routes;" << std::endl;
        std::cout << "|" << std::endl;
        std::cout << "| 7 - Search passenger;" << std::endl;
        std::cout << "| 8 - Search ticket;" << std::endl;
        std::cout << "| q - QUIT;" << std::endl;
        std::cout << "+------------------------+" << std::endl;
        std::cout << "Enter your choice from the menu: ";
    }

    void printPassenger(const Passenger& p, const Ticket& t) const {
        std::cout << "+---------------------------------------------------------+" << std::endl;
        std::cout << "| Passenger ID: " << t.passenger_id << std::endl;
        std::cout << "| Passenger name: " << p.name << std::endl;
        std::cout << "| Passenger surname: " << p.surname << std::endl;
        std::cout << "| Passenger phone number: " << p.phone << std::endl;
        std::cout << "| Passenger passport ID: " << p.passportId << std::endl;
        std::cout << "+---------------------------------------------------------+" << std::endl;
    }

    void printTicket(const Ticket& t, const Passenger& p) const {
        std::cout << "+-------------------------------------------------------------+" << std::endl;
        std::cout << "| Ticket ID: " << t.id << std::endl;
        std::cout << "| Passenger ID: " << t.passenger_id << std::endl;
        std::cout << "| Passenger name: " << p.name << std::endl;
        std::cout << "| Passenger surname: " << p.surname << std::endl;
        std::cout << "| Train ID: " << t.train_id << std::endl;
        std::cout << "| Carriage ID: " << t.carriage_id << std::endl;
        std::cout << "| Seat number: " << t.seat_number << std::endl;
        std::cout << "| Ticket price: " << t.price << std::endl;
        std::cout << "| Purchase date: " << t.purchase_date << std::endl;
        std::cout << "+-------------------------------------------------------------+" << std::endl;
    }

    void printRoute(const Route& r) const {
        std::cout << "+-------------------------------------------------------------+" << std::endl;
        std::cout << "| Departure station: " << r.departure_station_name << std::endl;
        std::cout << "| Arrival station: " << r.arrival_station_name << std::endl;
        std::cout << "| Platform: " << r.platform_name << std::endl;
        std::cout << "| Departure time: " << r.departure_t << std::endl;
        std::cout << "| Arrival time: " << r.arrival_t << std::endl;
        std::cout << "+-------------------------------------------------------------+" << std::endl;
    }

    void dropWarning() {
        std::cout << "+--------------------------------------------------------+" << std::endl
                  << "|                   !!! WARNING !!!                      |" << std::endl
                  << "+--------------------------------------------------------+" << std::endl
                  << "| IF YOU PROCEED, ALL DATA FROM DATABASE WILL BE DELETED |" << std::endl
                  << "+--------------------------------------------------------+" << std::endl
                  << "|                               |                        |" << std::endl
                  << "|    0 - Cancel (RECOMMENDED)   |      1 - Delete        |" << std::endl
                  << "|                               |                        |" << std::endl
                  << "+--------------------------------------------------------+" << std::endl;
    }
};

#endif //INTERFACERENDER_H
