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
#include <cstdio>   // for std::snprintf

using namespace std;

namespace {
    const vector<string> ART = {
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

    const map<char,string> INNER_TOKEN = {
        {'7'," 7 "}, {'8'," 8 "}, {'9'," 9 "}, {'+'," + "},
        {'4'," 4 "}, {'5'," 5 "}, {'6'," 6 "}, {'-'," - "},
        {'1'," 1 "}, {'2'," 2 "}, {'3'," 3 "}, {'*'," * "},
        {'.'," . "}, {'0'," 0 "}, {'='," = "}, {'/'," / "}
    };

    pair<int,int> getTerminalSize() {
        struct winsize ws{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) return {24,80};
        return {static_cast<int>(ws.ws_row), static_cast<int>(ws.ws_col)};
    }

    void replace_all(string &str, const string &token, const string &replacement) {
        size_t pos = 0;
        while ((pos = str.find(token, pos)) != string::npos) {
            str.replace(pos, token.size(), replacement);
            pos += replacement.size();
        }
    }
}

// ----------------- Calc implementation -----------------
Calc::Calc()
: leftBuf(), rightBuf(), displayValue("0"), oper(0), enteringRight(false), shouldExit(false) {}

string Calc::button_bg(const string &s) {
    return string("\033[47m\033[30m") + s + "\033[0m";
}

void Calc::run() {
    clearScreen();
    hideCursor();

    // If you use a TermGuard or cursor_input that sets raw mode, make sure it's active.
    // Here we assume the system/other code sets terminal to raw; otherwise read() will wait for enter.
    draw(0);

    while (!shouldExit) {
        char ch = 0;
        ssize_t r = read(STDIN_FILENO, &ch, 1);
        if (r <= 0) continue;
        if (ch == '\r') ch = '\n';

        // backspace
        if (ch == 127 || ch == 8) {
            animatePress(0);
            if (enteringRight && !rightBuf.empty()) { rightBuf.pop_back(); displayValue = rightBuf.empty() ? "0" : rightBuf; }
            else if (!enteringRight && !leftBuf.empty()) { leftBuf.pop_back(); displayValue = leftBuf.empty() ? "0" : leftBuf; }
            draw(0);
            continue;
        }

        // q = quit
        if (ch == 'q' || ch == 'Q') { animatePress(0); shouldExit = true; break; }

        // c = clear
        if (ch == 'c' || ch == 'C') {
            animatePress(0);
            leftBuf.clear(); rightBuf.clear(); oper = 0; enteringRight = false; displayValue = "0";
            draw(0);
            continue;
        }

        // digits
        if ((ch >= '0' && ch <= '9') || ch == '.') {
            animatePress(ch);
            appendDigit(ch);
            draw(0);
            continue;
        }

        // operators
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            animatePress(ch);
            if (oper != 0 && !rightBuf.empty()) computeOnce();
            oper = ch;
            enteringRight = true;
            rightBuf.clear();
            displayValue = "0";
            draw(0);
            continue;
        }

        // compute
        if (ch == '=' || ch == '\n') {
            animatePress('=');
            computeOnce();
            draw(0);
            continue;
        }

        // ignore others
    }

    showCursor();
    clearScreen();
    move_cursor(1,1);
    cout << "Calculator closed.\n";
}

void Calc::draw(char highlightChar) {
    auto [rows, cols] = getTerminalSize();
    int artH = static_cast<int>(ART.size());
    int artW = static_cast<int>(ART[0].size());
    int startRow = max(1, (rows - artH) / 2 + 1);
    int startCol = max(1, (cols - artW) / 2 + 1);

    // clear first
    clearScreen();

    // TITLE (after clearing so it remains)
    string title =
        "  ██████╗ █████╗ ██╗      ██████╗██╗   ██╗██╗      █████╗ ████████╗ ██████╗ ██████╗ \n"
        " ██╔════╝██╔══██╗██║     ██╔════╝██║   ██║██║     ██╔══██╗╚══██╔══╝██╔═══██╗██╔══██╗\n"
        " ██║     ███████║██║     ██║     ██║   ██║██║     ███████║   ██║   ██║   ██║██████╔╝\n"
        " ██║     ██╔══██║██║     ██║     ██║   ██║██║     ██╔══██║   ██║   ██║   ██║██╔══██╗\n"
        " ╚██████╗██║  ██║███████╗╚██████╗╚██████╔╝███████╗██║  ██║   ██║   ╚██████╔╝██║  ██║\n"
        "  ╚═════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝\n";

    // split title into lines and center horizontally
    vector<string> titleLines;
    {
        istringstream iss(title);
        string l;
        while (getline(iss, l)) titleLines.push_back(l);
    }
    int titleH = static_cast<int>(titleLines.size());
    int titleW = 0;
    for (auto &l : titleLines) titleW = max(titleW, (int)l.size());
    int titleRow = max(1, startRow - titleH - 1);
    int titleCol = max(1, (cols - titleW) / 2 + 1);

    setTextColor(YELLOW);
    for (int i = 0; i < titleH; ++i) {
        move_cursor(titleCol, titleRow + i);
        cout << titleLines[i];
    }
    resetTextColor();

    // display value right-aligned
    string dv = displayValue;
    if ((int)dv.size() > DISPLAY_WIDTH) dv = "..." + dv.substr(dv.size() - DISPLAY_WIDTH + 3);
    if ((int)dv.size() < DISPLAY_WIDTH) dv = string(DISPLAY_WIDTH - dv.size(), ' ') + dv;

    // draw art centered
    for (int i = 0; i < artH; ++i) {
        string line = ART[i];
        size_t pos = line.find("{d}");
        if (pos != string::npos) line.replace(pos, 3, dv);

        if (highlightChar != 0 && INNER_TOKEN.count(highlightChar)) {
            string token = INNER_TOKEN.at(highlightChar);
            string colored = button_bg(token);
            replace_all(line, token, colored);
        }

        move_cursor(startCol, startRow + i);
        cout << line;
    }

    // footer
    string footer = " q:quit  c:clear  Backspace:del  Enter/= : compute ";
    int footerRow = startRow + artH + 1;
    int footerCol = max(1, (cols - static_cast<int>(footer.size())) / 2 + 1);
    move_cursor(footerCol, footerRow);
    cout << footer << flush;
}

void Calc::animatePress(char ch) {
    if (ch != 0) {
        draw(ch);
        this_thread::sleep_for(chrono::milliseconds(120));
        draw(0);
    } else {
        // tiny visual nudge
        draw(0);
        this_thread::sleep_for(chrono::milliseconds(60));
        draw(0);
    }
}

void Calc::appendDigit(char d) {
    if (!enteringRight) {
        if (d == '.' && leftBuf.find('.') != string::npos) return;
        leftBuf.push_back(d);
        displayValue = leftBuf.empty() ? "0" : leftBuf;
    } else {
        if (d == '.' && rightBuf.find('.') != string::npos) return;
        rightBuf.push_back(d);
        displayValue = rightBuf.empty() ? "0" : rightBuf;
    }
}

void Calc::computeOnce() {
    if (oper == 0) return;
    double a = leftBuf.empty() ? 0.0 : std::stod(leftBuf);
    double b = rightBuf.empty() ? a : std::stod(rightBuf);
    double res = 0.0;
    bool ok = true;
    switch (oper) {
        case '+': res = a + b; break;
        case '-': res = a - b; break;
        case '*': res = a * b; break;
        case '/':
            if (std::fabs(b) < 1e-12) ok = false;
            else res = a / b;
            break;
        default: ok = false; break;
    }
    if (!ok) {
        displayValue = "Error";
    } else {
        if (std::fabs(res - std::round(res)) < 1e-9) {
            displayValue = std::to_string((long long)std::llround(res));
        } else {
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%.8g", res);   // <- correct literal here
            displayValue = std::string(buf);
        }
        leftBuf = displayValue;
        rightBuf.clear();
        oper = 0;
        enteringRight = false;
    }
}
