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

// draw calculator
extern std::vector<std::string> calculator;
extern std::map<char, std::string> innerToken;

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
    std::string button_bg(const std::string &s);

    // void clearHistory();
    // void printHistory() const;
};