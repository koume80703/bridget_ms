#pragma once

#include <string>

enum class Color { BLACK = -1, WHITE = 1, EMPTY = 0, NONE = -255 };
enum class Player { BLACK = -1, WHITE = 1, DRAW = 0, NONE = -255 };

inline string color_to_string(Color c) {
    if (c == Color::BLACK) {
        return "BLACK";
    } else if (c == Color::WHITE) {
        return "WHITE";
    } else if (c == Color::EMPTY) {
        return "EMPTY";
    } else if (c == Color::NONE) {
        return "NONE";
    }
    return "";
}
