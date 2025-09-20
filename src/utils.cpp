// ultils.cpp
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

using namespace std;

string formatTime(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    ostringstream oss;
    oss << setw(2) << setfill('0') << minutes
        << ":" << setw(2) << setfill('0') << seconds;
    return oss.str();
}

void play_sound(const string& path) {
#ifdef _WIN32
    PlaySound(path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
#else
    // Linux: use aplay or similar command
    string command = "aplay -q " + path + " &";
    system(command.c_str());
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H"; // Clear screen + move cursor to top-left
#endif
}

void clearTerminal() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033c"; // Full reset
#endif
}

void setTextColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    cout << "\033[" << color << "m";
#endif
}

void move_cursor(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y << ";" << x << "H"; // Move cursor
#endif
}


void hideCursor() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
#else
    cout << "\033[?25l";
#endif
}

void showCursor() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hOut, &cursorInfo);
#else
    cout << "\033[?25h";
#endif
}

int getch() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

bool kbhit() {
#ifdef _WIN32
    return _kbhit();
#else
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
#endif
}

void console_size(int width, int height) {
#ifdef _WIN32
    string cmd = "mode con: cols=" + to_string(width) + " lines=" + to_string(height);
    system(cmd.c_str());
#else
    // Linux: usually can't resize terminal from code safely
    // Maybe print a warning or leave empty
#endif
}

void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

int random_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Box drawing characters
const char SYMBOL_HORIZONTAL[]   = "\u2500"; // ─
const char SYMBOL_VERTICAL[]     = "\u2502"; // │
const char SYMBOL_TOP_LEFT[]     = "\u250C"; // ┌
const char SYMBOL_TOP_RIGHT[]    = "\u2510"; // ┐
const char SYMBOL_BOTTOM_LEFT[]  = "\u2514"; // └
const char SYMBOL_BOTTOM_RIGHT[] = "\u2518"; // ┘
const char SYMBOL_T_LEFT[]       = "\u251C"; // ├
const char SYMBOL_T_RIGHT[]      = "\u2524"; // ┤
const char SYMBOL_T_TOP[]        = "\u252C"; // ┬
const char SYMBOL_T_BOTTOM[]     = "\u2534"; // ┴
const char SYMBOL_INTERSECT[]    = "\u253C"; // ┼


// Double line box drawing characters
const char SYMBOL_DOUBLE_HORIZONTAL[]   = "\u2550"; // ═
const char SYMBOL_DOUBLE_VERTICAL[]     = "\u2551"; // ║
const char SYMBOL_DOUBLE_TOP_LEFT[]     = "\u2554"; // ╔
const char SYMBOL_DOUBLE_TOP_RIGHT[]    = "\u2557"; // ╗
const char SYMBOL_DOUBLE_BOTTOM_LEFT[]  = "\u255A"; // ╚    
const char SYMBOL_DOUBLE_BOTTOM_RIGHT[] = "\u255D"; // ╝
const char SYMBOL_DOUBLE_T_LEFT[]       = "\u2560"; // ╠
const char SYMBOL_DOUBLE_T_RIGHT[]      = "\u2563"; // ╣
const char SYMBOL_DOUBLE_T_TOP[]        = "\u2566"; // ╦
const char SYMBOL_DOUBLE_T_BOTTOM[]     = "\u2569"; // ╩
const char SYMBOL_DOUBLE_INTERSECT[]    = "\u256C"; // ╬

// Block characters
const char BLOCK_FULL[]   = "\u2588"; // █
const char BLOCK_HALF[]   = "\u2592"; // ▒
const char BLOCK_RECT[]   = "\u2585"; // ▅
const char BLOCK_UP[]     = "\u2580"; // ▀
const char BLOCK_DOWN[]   = "\u2584"; // ▄
const char BLOCK_LEFT[]   = "\u258C"; // ▌
const char BLOCK_RIGHT[]  = "\u2590"; // ▐

// Arrow characters
const char* ARROW_UP    = "\u2191"; // ↑
const char* ARROW_DOWN  = "\u2193"; // ↓
const char* ARROW_LEFT  = "\u2190"; // ←
const char* ARROW_RIGHT = "\u2192"; // →

// Heart characters
const char* HEART_SOLID  = "\u2665"; // ♥
const char* HEART_EMPTY  = "\u2661"; // ♡

// Triangle characters
const char* TRIANGLE_UP    = "\u25B2"; // ▲
const char* TRIANGLE_DOWN  = "\u25BC"; // ▼
const char* TRIANGLE_LEFT  = "\u25C0"; // ◀
const char* TRIANGLE_RIGHT = "\u25B6"; // ▶

// Ball characters
const char* BALL_SOLID    = "\u25CF"; // ●
const char* BALL_HOLLOW   = "\u25CB"; // ○
const char* BALL_BULLSEYE = "\u25CE"; // ◎