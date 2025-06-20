
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <windows.h>
#include <ctime>
#include <thread>
#include <conio.h>
#include <regex>

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

inline bool isValidDateTime(const std::string& datetime) {
    std::regex pattern(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
    return std::regex_match(datetime, pattern);
}

void pressAnyKeyToContinue() {
    std::cout << "Press any key to continue...";
    _getch();
    std::cout << std::endl;
}

template<typename T>
bool safeInput(T& variable, const std::string& prompt = "Enter value: ") {
    std::cout << prompt;
    std::cin >> variable;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Invalid input. Please enter a valid value.\n";
        return false;
    }

    return true;
}

inline bool safeStringInput(std::string& str, const std::string& prompt = "Enter text: ") {
    std::cout << prompt;
    std::getline(std::cin, str);

    if (str.empty()) {
        std::cerr << "Input cannot be empty.\n";
        return false;
    }

    return true;
}

inline bool safeDateTimeInput(std::string& datetime, const std::string& prompt = "Enter date (YYYY-MM-DD HH:MM:SS): ") {
    std::cout << prompt;
    std::getline(std::cin, datetime);

    if (!isValidDateTime(datetime)) {
        std::cerr << "Invalid date format. Please use: YYYY-MM-DD HH:MM:SS\n";
        return false;
    }

    return true;
}

#endif //FUNCTIONS_H
