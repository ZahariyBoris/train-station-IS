
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <windows.h>
#include <ctime>
#include <thread>
#include <conio.h>

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void sleepAndClear(int val) {
    std::this_thread::sleep_for(std::chrono::milliseconds(val));
    clearScreen();
}

std::string getCurrentDateTime() {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

void pressAnyKeyToContinue() {
    std::cout << "Press any key to continue...";
    _getch();
    std::cout << std::endl;
}

#endif //FUNCTIONS_H
