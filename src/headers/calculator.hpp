
// calculator.hpp
#pragma once

#include <string>

class Calc {
public:
    Calc();
    ~Calc() = default;

    // entry point
    void run();

    // drawing (public for testing)
    void draw(char highlight = 0);

private:
    // state
    std::string leftBuf;
    std::string rightBuf;
    std::string displayValue;
    char oper;
    bool enteringRight;
    bool shouldExit;

    // helpers
    void animatePress(char ch);
    void appendDigit(char d);
    void computeOnce();

    static std::string button_bg(const std::string &s);
    static constexpr int DISPLAY_WIDTH = 15;
};
