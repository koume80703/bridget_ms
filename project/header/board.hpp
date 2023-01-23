#pragma once

using namespace std;

#include <iostream>
#include <vector>

#include "action.hpp"
#include "cell.hpp"
#include "parameter.hpp"
#include "piece.hpp"

class Board {
   public:
    static const int BOARD_SIZE = board::BOARD_SIZE;
    static const int BOARD_HEIGHT = board::BOARD_HEIGHT;
    static const int ABS_C_SIZE = board::ABS_C_SIZE;

    static const bool DEBUG_BOARD = false;

    // タプルから値を取得する際にstd::get<N>();を利用するが、この際にNに直接値を入れるとxyz座標のどれを取得したいのか分かりづらいので、この値を用いることで可読性を上げる。
    enum { XC = 0, YC, ZC };

    Board();

    Board(const Board &rhs);
    Board &operator=(const Board &rhs);

    vector<Action> legal_actions() const;

    bool set_stone(const Action act, const Color color, const int turn = -1);

    void show_all_board(const bool horizontal = false) const;
    void show_board_1f(const int z) const;
    void show_board_horizontal() const;

    Color get_color(const int x, const int y, const int z) const;
    int get_index(const int x, const int y, const int z) const;

   private:
    Cell SolidBoard[BOARD_HEIGHT][BOARD_SIZE][BOARD_SIZE];  // 3次元配列

    void get_abs_coordinate(tuple<int, int, int> *abs_c,
                            const Action act) const;
    tuple<int, int, int> get_base_block(const Action act) const;

    bool is_on_ground(const Action act) const;
    bool is_having_space(const Action act) const;
    bool is_duplicated(const Action act) const;
    bool is_inside(const Action act) const;
    bool placable_cell(const Action act) const;
};
