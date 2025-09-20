#pragma once 

#include <string>
#include <vector>
#include <utility> // for std::pair
#include <iostream>
#include <cstdlib>


class Calc {
private:
    std::vector<std::string> history;
    std::string currentInput;
    // pair<double, double> lastOperands;

public:
    Calc();
    void run();
    void processInput(const std::string& input);
    // double squareRoot(double value);
    void clearHistory();
    void printHistory() const;
}