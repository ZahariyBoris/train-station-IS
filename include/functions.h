
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <windows.h>
#include <thread>

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

#endif //FUNCTIONS_H
