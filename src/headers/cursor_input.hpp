#pragma once
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
// Input control keys
// const int KEY_UP = 65;    // Arrow Up
// const int KEY_DOWN = 66;  // Arrow Down
// const int KEY_LEFT = 68;  // Arrow Left
// const int KEY_RIGHT = 67; // Arrow Right
// const int KEY_ENTER = 10; // Enter key
// const int KEY_ESCAPE = 27; // Escape key

enum class InputKey {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    ESC,
    Q, // Quit to menu
    R,  // Restart game
    LEFT_BRACKET, // '[' key
    RIGHT_BRACKET, // ']' key
    SPACE // ' ' key    
};

// Play sound effects
// enum SoundEffect {
//     CLICK,
//     VICTORY,
//     DEFEAT
// };

// void playSound(SoundEffect effect);
void setTerminalNonBlocking();
void restoreTerminalBlocking();

InputKey getInputKey();