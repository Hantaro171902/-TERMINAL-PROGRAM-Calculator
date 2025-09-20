#pragma once 

#include <string>
#include <vector>
#include <utility> // for std::pair
#include <iostream>
#include <cstdlib>
#include "utils.hpp"

class Calc {
private:
    std::vector<std::string> history;
    std::string currentInput;
    // pair<double, double> lastOperands;

public:
    void run();
    void processInput(double a, double b, char op);
    // double squareRoot(double value);
    void display();
    void clearHistory();
    void printHistory() const;
}