#pragma once

using namespace std;

#include <iostream>
#include <utility>

#include "piece.hpp"

class Action {
   private:
    pair<int, int> xy;
    // 基準小体のある座標、z座標について合法手となるように一意に決める。

    Piece piece;

   public:
    Action();
    Action(int x, int y, Piece piece);

    Action(const Action& rhs);
    Action& operator=(const Action& rhs);

    bool operator==(const Action& rhs) const;

    pair<int, int> get_xy() const;
    const Piece& get_piece() const;

    string get_string() const;
};