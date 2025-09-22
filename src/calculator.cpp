#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include "calculator.hpp"
#include "color.hpp"
#include "utils.hpp"

using namespace std;

// small helpers and calculator state
string leftBuf = "";
string rightBuf = "";
char oper = 0;
bool enteringRight = false;
string displayValue = "0";
bool shouldExit = false;

// Definitions for global variables declared in calculator.hpp
std::vector<std::string> calculator = {
    "\t\t\t    _____________________ ",
    "\t\t\t  /|  _________________  |",
    "\t\t\t | | | {d} | |",
    "\t\t\t | | |_________________| |",
    "\t\t\t | |  ___ ___ ___   ___  |",
    "\t\t\t | | | 7 | 8 | 9 | | + | |",
    "\t\t\t | | |___|___|___| |___| |",
    "\t\t\t | | | 4 | 5 | 6 | | - | |",
    "\t\t\t | | |___|___|___| |___| |",
    "\t\t\t | | | 1 | 2 | 3 | | * | |",
    "\t\t\t | | |___|___|___| |___| |",
    "\t\t\t | | | . | 0 | = | | / | |",
    "\t\t\t | | |___|___|___| |___| |",
    "\t\t\t | |_____________________|",
    "\t\t\t |/_____________________/ "
};

std::map<char, std::string> innerToken = {
    {'7'," 7 "}, {'8'," 8 "}, {'9'," 9 "}, {'+'," + "},
    {'4'," 4 "}, {'5'," 5 "}, {'6'," 6 "}, {'-'," - "},
    {'1'," 1 "}, {'2'," 2 "}, {'3'," 3 "}, {'*'," * "},
    {'.'," . "}, {'0'," 0 "}, {'='," = "}, {'/'," / "}
};

string Calc::button_bg (const string &s) {
    return string("\033[47m\033[30m") + s + "\033[0m";
}

struct TermGuard {
    termios oldt;
    bool enabled = false;
    void enableRaw() {
        if (enabled) return;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~(ECHO | ICANON);
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        enabled = true;
    }
    void disableRaw() {
        if (!enabled) return;
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        enabled = false;
    }
    ~TermGuard(){ disableRaw(); }
};

void Calc::run() {
    clearTerminal();
    TermGuard tg;
    tg.enableRaw();
    draw(0, displayValue);

    while (!shouldExit) {
        char ch = 0;
        ssize_t r = read(STDIN_FILENO, &ch, 1);
        if (r <= 0) break;
        if (ch == '\r') ch = '\n';

        // backspace
        if (ch == 127 || ch == 8) {
            // animate a generic visual feedback by highlighting none but redrawing
            animatePress(0);
            if (enteringRight && !rightBuf.empty()) {
                rightBuf.pop_back();
                displayValue = rightBuf.empty() ? "0" : rightBuf;
            } else if (!enteringRight && !leftBuf.empty()) {
                leftBuf.pop_back();
                displayValue = leftBuf.empty() ? "0" : leftBuf;
            }
            continue;
        }

        if (ch == 'q' || ch == 'Q') {
            animatePress(0);
            shouldExit = true;
            break;
        }

        if (ch == 'c' || ch == 'C') {
            animatePress(0);
            leftBuf.clear(); rightBuf.clear(); oper = 0; enteringRight = false; displayValue = "0";
            draw(0, displayValue);
            continue;
        }

        if ((ch >= '0' && ch <= '9') || ch == '.') {
            appendDigit(ch);
            animatePress(ch);
            continue;
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            animatePress(ch);
            if (oper != 0 && !rightBuf.empty()) computeOnce();
            oper = ch;
            enteringRight = true;
            rightBuf.clear();
            displayValue = "0";
            draw(0, displayValue);
            continue;
        }

        if (ch == '=' || ch == '\n') {
            animatePress('=');
            computeOnce();
            draw(0, displayValue);
            continue;
        }

        // ignore others
    }

    tg.disableRaw();
    clearScreen();
    cout << "Calculator closed.\n";
}

// Draw calculator; highlightChar==0 means no highlight
void Calc::draw(char highlightChar = 0, const string &displayValue = "0") {
    clearScreen();
    clearTerminal();
    int startRow = 2; // or another suitable value
    int cols = 80;    

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

    cout <<"\n\n\n\n";
    // prepare display string right-aligned
    string dv = displayValue;
    if ((int)dv.size() > DISPLAY_WIDTH) {
        dv = "..." + dv.substr(dv.size() - DISPLAY_WIDTH + 3);
    } 

    if ((int)dv.size() < DISPLAY_WIDTH) {
        dv = string(DISPLAY_WIDTH - dv.size(), ' ') + dv;
    }

    for (const string &rawLine : calculator) {
        string line = rawLine;
        // replace {d} in the specific line
        size_t pos = line.find("{d}");
        if (pos != string::npos) {
            line.replace(pos, 3, dv);
        }
        // If we need to highlight a token, replace ALL occurrences of the exact token " X "
        if (highlightChar != 0 && innerToken.count(highlightChar)) {
            string token = innerToken[highlightChar];
            string colored = button_bg(token);
            // Replace all non-overlapping occurrences of token in line
            size_t search = 0;
            while ((search = line.find(token, search)) != string::npos) {
                line.replace(search, token.size(), colored);
                // advance past the inserted colored text to avoid recursive find on same area
                search += colored.size();
            }
        }
        cout << line << endl;
    }

    cout << "\n q:quit  c:clear  Backspace:del  Enter/= : compute\n";
}

void Calc::animatePress(char ch) {
    // highlight inner cell, hold briefly, then redraw normally
    draw(ch, displayValue);
    this_thread::sleep_for(chrono::milliseconds(120));
    draw(0, displayValue);
}

void Calc::appendDigit(char d) {
    if (!enteringRight) {
        if (!(d=='.' && leftBuf.find('.')!=string::npos)) leftBuf.push_back(d);
        displayValue = leftBuf.empty() ? "0" : leftBuf;
    } else {
        if (!(d=='.' && rightBuf.find('.')!=string::npos)) rightBuf.push_back(d);
        displayValue = rightBuf.empty() ? "0" : rightBuf;
    }
}

void Calc::computeOnce() {
    if (oper == 0) return;
    double a = leftBuf.empty() ? 0.0 : stod(leftBuf);
    double b = rightBuf.empty() ? a : stod(rightBuf);
    double res = 0.0;
    bool ok = true;
    if (oper == '+') res = a + b;
    else if (oper == '-') res = a - b;
    else if (oper == '*') res = a * b;
    else if (oper == '/') {
        if (fabs(b) < 1e-12) ok = false;
        else res = a / b;
    } else ok = false;

    if (!ok) displayValue = "Error";
    else {
        // pretty format
        if (fabs(res - round(res)) < 1e-9) displayValue = to_string((long long)llround(res));
        else {
            char buf[64];
            snprintf(buf, sizeof(buf), "%.8g", res);
            displayValue = string(buf);
        }
        leftBuf = displayValue;
        rightBuf.clear();
        oper = 0;
        enteringRight = false;
    }
}

// void Calc::display() {
//     clearTerminal();
//     cout << R"(    _____________________
//                  /|  _________________  |
//                 | | |              0. | |
//                 | | |_________________| |
//                 | |  ___ ___ ___   ___  |
//                 | | | 7 | 8 | 9 | | + | |
//                 | | |___|___|___| |___| |
//                 | | | 4 | 5 | 6 | | - | |
//                 | | |___|___|___| |___| |
//                 | | | 1 | 2 | 3 | | x | |
//                 | | |___|___|___| |___| |
//                 | | | . | 0 | = | | / | |
//                 | | |___|___|___| |___| |
//                 | |_____________________|
//                 |/_____________________/
//             )" << endl;
// }

