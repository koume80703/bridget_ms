#pragma once

using namespace std;

#include "enum.hpp"

class Cell {
   private:
    int index;  // このマスに配置された駒が何番目に配置されたものであるか
    Color color;  // このマスが白であるか黒であるか空であるか

   public:
    Cell();
    Cell(int index, Color color);

    Cell(const Cell &rhs);
    Cell &operator=(const Cell &rhs);

    int get_index() const;
    Color get_color() const;
};