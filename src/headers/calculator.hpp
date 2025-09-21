#pragma once 

#include <string>
#include <vector>
#include <utility> // for std::pair
#include <iostream>
#include <cstdlib>
#include <map>
#include <chrono>
#include <cmath>
#include <thread>
#include "utils.hpp"

//helper
std::string button_bg(const std::string &s);

// draw calculator
std::vector<std::string> calculator = {
    "   _____________________ ",
    " /|  _________________  |",
    "| | | {d} | |",
    "| | |_________________| |",
    "| |  ___ ___ ___   ___  |",
    "| | | 7 | 8 | 9 | | + | |",
    "| | |___|___|___| |___| |",
    "| | | 4 | 5 | 6 | | - | |",
    "| | |___|___|___| |___| |",
    "| | | 1 | 2 | 3 | | * | |",
    "| | |___|___|___| |___| |",
    "| | | . | 0 | = | | / | |",
    "| | |___|___|___| |___| |",
    "| |_____________________|",
    "|/_____________________/ "
};

std::map<char, std::string> innerToken = {
    {'7'," 7 "}, {'8'," 8 "}, {'9'," 9 "}, {'+'," + "},
    {'4'," 4 "}, {'5'," 5 "}, {'6'," 6 "}, {'-'," - "},
    {'1'," 1 "}, {'2'," 2 "}, {'3'," 3 "}, {'*'," * "},
    {'.'," . "}, {'0'," 0 "}, {'='," = "}, {'/'," / "}
};

class Calc {
private:
    std::vector<std::string> history;
    std::string currentInput;
    int DISPLAY_WIDTH = 15;
    // pair<double, double> lastOperands;

public:
    void run();
    // void processInput(double a, double b, char op);
    // double squareRoot(double value);
    void draw(char highlightChar, const std::string &displayValue);
    void animatePress(char ch);
    void appendDigit(char d);
    void computeOnce();
    void display();
    // void clearHistory();
    // void printHistory() const;
}