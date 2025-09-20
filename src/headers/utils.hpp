// ultils.hpp
#pragma once

#include <iostream>
#include <string>

void clearScreen();
void clearTerminal(); // Full terminal reset

// void setTextColor(int color);
void hideCursor();
void showCursor();

int getch();    // For key input
bool kbhit();   // For non-blocking keyboard input

void console_size(int width, int height);
void sleep_ms(int ms);
int random_range(int min, int max);
std::string formatTime(int totalSeconds);

void move_cursor(int x, int y);
void play_sound(const std::string& path);


// Box drawing characters (UTF-8)
extern const char SYMBOL_HORIZONTAL[];
extern const char SYMBOL_VERTICAL[];
extern const char SYMBOL_TOP_LEFT[];
extern const char SYMBOL_TOP_RIGHT[];
extern const char SYMBOL_BOTTOM_LEFT[];
extern const char SYMBOL_BOTTOM_RIGHT[];
extern const char SYMBOL_T_LEFT[];
extern const char SYMBOL_T_RIGHT[];
extern const char SYMBOL_T_TOP[];
extern const char SYMBOL_T_BOTTOM[];
extern const char SYMBOL_INTERSECT[];

// Double line box drawing characters (UTF-8)
extern const char SYMBOL_DOUBLE_HORIZONTAL[];
extern const char SYMBOL_DOUBLE_VERTICAL[];
extern const char SYMBOL_DOUBLE_TOP_LEFT[];         
extern const char SYMBOL_DOUBLE_TOP_RIGHT[];
extern const char SYMBOL_DOUBLE_BOTTOM_LEFT[];
extern const char SYMBOL_DOUBLE_BOTTOM_RIGHT[];
extern const char SYMBOL_DOUBLE_T_LEFT[];
extern const char SYMBOL_DOUBLE_T_RIGHT[];
extern const char SYMBOL_DOUBLE_T_TOP[];
extern const char SYMBOL_DOUBLE_T_BOTTOM[];
extern const char SYMBOL_DOUBLE_INTERSECT[];

extern const char BLOCK_FULL[];
extern const char BLOCK_HALF[];
extern const char BLOCK_RECT[];

extern const char* ARROW_UP;
extern const char* ARROW_DOWN;
extern const char* ARROW_LEFT;
extern const char* ARROW_RIGHT;

extern const char* HEART_SOLID;
extern const char* HEART_EMPTY;

extern const char* TRIANGLE_UP;
extern const char* TRIANGLE_DOWN;
extern const char* TRIANGLE_LEFT;
extern const char* TRIANGLE_RIGHT;

extern const char* BALL_SOLID;
extern const char* BALL_HOLLOW;
extern const char* BALL_BULLSEYE;