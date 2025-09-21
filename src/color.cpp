#include "color.hpp"
#include <iostream>

using namespace std;

void setTextColor(TextColor color) {
    cout << "\033[" << color << "m";
}

void resetTextColor() {
    cout << "\033[0m";
}
