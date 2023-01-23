#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <utility>

#include "action.hpp"
#include "board.hpp"
#include "enum.hpp"
#include "parameter.hpp"

using namespace std;

class Game {
   public:
    static const Player START_PLAYER = Player::BLACK;
    static const Player TARGET_PLAYER = START_PLAYER;

    static const bool DEBUG_GAME = game::DEBUG_GAME;

    enum DIRECTION { X = 1, Y = -1 };

    Game();

    Game(const Game &rhs);
    Game &operator=(const Game &rhs);

    int get_result(const Player target) const;

    vector<Action> legal_actions() const;

    void shift_player();
    bool set_stone(const Action act);
    void pass_moving();
    void end_game(bool connected);

    bool is_finished() const;

    bool is_win(const Player player) const;
    bool is_lose(const Player player) const;
    bool is_draw() const;

    bool is_start_player() const;
    Color is_connected() const;

    int get_piece_left(const int kind, const Player player) const;

    Player get_cp() const;
    Player get_next_player() const;
    Player get_winner() const;

    const Board &get_board() const;
    int get_turn() const;

    Color get_top_view_cell(int x, int y) const;

    bool get_was_passed() const;

    void show_all() const;
    void show_action(const Action act) const;
    void show_board() const;
    void show_top_view() const;
    void show_player_status() const;

    static const Action random_action(vector<Action> la) {
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> rid(0, la.size() - 1);
        return la[rid(mt)];
    }

    static const Action human_action(vector<Action> la) {
        Action act;
        Piece p;

        int x, y;
        int kind, rdx, rdy;

        bool found = false;

        do {
            cin >> x >> y >> kind >> rdx >> rdy;

            p = Piece(kind, rdx, rdy);
            act = Action(x, y, p);

            found = find(la.begin(), la.end(), act) != la.end();
        } while (!found);

        return act;
    }

   private:
    int turn;
    Board board;

    // 各プレイヤーが持つ駒の数
    // 配列の要素を順にLOSTに割り当て、また、その要素のfirstを黒、secondを白として
    // 駒の残り数を記憶する。
    pair<int, int> piece_left[Piece::PIECE_KIND];

    // 上から見た3次元盤面
    Color top_view[Board::BOARD_SIZE][Board::BOARD_SIZE];

    Player current_player, winner;

    bool was_passed;

    Player color_to_player(Color color) const;
    Color player_to_color(Player player) const;

    void update_top_view();
    void init_top_view();

    void init_piece_left();

    pair<int, int> count_pieces() const;

    bool explore_next(const int base_x, const int base_y,
                      bool accessed[][Board::BOARD_SIZE], DIRECTION D) const;
};