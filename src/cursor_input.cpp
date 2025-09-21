#include "cursor_input.hpp"


static struct termios original_termios;
static int original_flags;

void setTerminalNonBlocking() {
    tcgetattr(STDIN_FILENO, &original_termios);
    original_flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    struct termios newt = original_termios;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fcntl(STDIN_FILENO, F_SETFL, original_flags | O_NONBLOCK);
}

void restoreTerminalBlocking() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    fcntl(STDIN_FILENO, F_SETFL, original_flags);
}


InputKey getInputKey() {
    // termios oldt{}, newt{};
    // tcgetattr(STDIN_FILENO, &oldt);
    // newt = oldt;
    // newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    // tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // // Set non-blocking
    // int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    // fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    InputKey key = InputKey::NONE;
    char ch;

    if (read(STDIN_FILENO, &ch, 1) == 1) {
        if (ch == '\033') { // Escape sequence
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
                read(STDIN_FILENO, &seq[1], 1) == 1) {
                switch (seq[1]) {
                    case 'A': key = InputKey::UP; break;
                    case 'B': key = InputKey::DOWN; break;
                    case 'C': key = InputKey::RIGHT; break;
                    case 'D': key = InputKey::LEFT; break;
                }
            }
        } else {
            switch (ch) {
                case 'w': case 'W': key = InputKey::UP; break;
                case 's': case 'S': key = InputKey::DOWN; break;
                case 'a': case 'A': key = InputKey::LEFT; break;
                case 'd': case 'D': key = InputKey::RIGHT; break;
                case '\n': key = InputKey::ENTER; break;
                case 27:   key = InputKey::ESC; break; // Escape
                case 'q': case 'Q': key = InputKey::Q; break;
                case 'r': case 'R': key = InputKey::R; break;
                case ' ': key = InputKey::SPACE; break; // Space key
                case '[': key = InputKey::LEFT_BRACKET; break;
                case ']': key = InputKey::RIGHT_BRACKET; break;
                default: key = InputKey::NONE; break;
            }
        }
    }

    // // Restore settings
    // tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // fcntl(STDIN_FILENO, F_SETFL, oldf);

    return key;
}

// void playSound(SoundEffect effect) {
// #ifdef _WIN32
//     // Not implemented on Windows in this file
// #else
//     switch (effect) {
//         case CLICK:   system("aplay -q assets/sounds/click.wav &"); break;
//         case VICTORY: system("aplay -q assets/sounds/victory.wav &"); break;
//         case DEFEAT:  system("aplay -q assets/sounds/defeat.wav &"); break;
//     }
// #endif
// }

