// calculator.cpp
#include "calculator.hpp"
#include "utils.hpp"   // expects: clearScreen(), move_cursor(col,row), hideCursor(), showCursor(), setTextColor(), resetTextColor()
#include "color.hpp"   // expects color enums like YELLOW
#include <vector>
#include <map>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <thread>
#include <sstream>
#include <cmath>
#include "calculator.hpp"

using namespace std;

void Calc::run() {
    clearTerminal();
    printWelcomeMessage();
    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        if (input == "q" || input == "Q") {
            cout << "Exiting calculator. Goodbye!" << endl;
            break;
        } else if (input == "h" || input == "H") {
            printWelcomeMessage();
        } else if (input == "c" || input == "C") {
            clearHistory();
            cout << "History cleared." << endl;
        } else {
            // Parse and evaluate the expression
            istringstream iss(input);
            double a, b;
            char op;
            if (iss >> a >> op >> b) {
                processInput(a, b, op);
                history.push_back(input);
            } else {
                cout << "Invalid input. Please enter a valid expression or command." << endl;
            }
        }
    }
}

void Calc::display() {
    clearTerminal();
    cout << R"(    _____________________
                 /|  _________________  |
                | | |              0. | |
                | | |_________________| |
                | |  ___ ___ ___   ___  |
                | | | 7 | 8 | 9 | | + | |
                | | |___|___|___| |___| |
                | | | 4 | 5 | 6 | | - | |
                | | |___|___|___| |___| |
                | | | 1 | 2 | 3 | | x | |
                | | |___|___|___| |___| |
                | | | . | 0 | = | | / | |
                | | |___|___|___| |___| |
                | |_____________________|
                |/_____________________/
            )" << endl;
    printHistory();
}

void Calc::processInput(double a, double b, char op) {
    // Process the user input and perform calculations
    // This is a placeholder implementation
    switch (op)
    {
    case '+':
        cout << a + b << endl;
        break;
    case '-':
        cout << a - b << endl;
        break;
    case '*':
        cout << a * b << endl;
        break;
    case '/':
        if (b != 0) {
            cout << a / b << endl;
        } else {
            cout << "Error: Division by zero" << endl;
        }
        break;
    default:
        break;
    }
}