#include "game.hpp"

Game::Game() {
    board = Board();
    current_player = START_PLAYER;
    turn = 0;
    winner = Player::NONE;
    was_passed = false;

    init_piece_left();

    init_top_view();
}

Game::Game(const Game &rhs) {
    board = rhs.board;
    current_player = rhs.current_player;
    turn = rhs.turn;
    winner = rhs.winner;
    was_passed = rhs.was_passed;

    for (int k = 0; k < Piece::PIECE_KIND; k++) {
        piece_left[k].first = rhs.piece_left[k].first;
        piece_left[k].second = rhs.piece_left[k].second;
    }

    init_top_view();
}

Game &Game::operator=(const Game &rhs) {
    board = rhs.board;
    current_player = rhs.current_player;
    turn = rhs.turn;
    winner = rhs.winner;
    was_passed = rhs.was_passed;

    for (int k = 0; k < Piece::PIECE_KIND; k++) {
        piece_left[k].first = rhs.piece_left[k].first;
        piece_left[k].second = rhs.piece_left[k].second;
    }

    init_top_view();

    return *this;
}

int Game::get_result(const Player target) const {
    if (is_win(target)) {
        return 1;
    } else if (is_lose(target)) {
        return -1;
    } else {
        return 0;
    }
}

vector<Action> Game::legal_actions() const {
    vector<Action> la = board.legal_actions();

    vector<Action> new_la;

    // 現在のプレイヤーの持ち駒を配列に保存する。
    int pl[Piece::PIECE_KIND];
    if (current_player == Player::BLACK) {
        for (int k = 0; k < Piece::PIECE_KIND; k++) {
            pl[k] = piece_left[k].first;
        }
    } else if (current_player == Player::WHITE) {
        for (int k = 0; k < Piece::PIECE_KIND; k++) {
            pl[k] = piece_left[k].second;
        }
    } else {
        cout << "error: Illegal value of current_player in "
                "Game::legal_actions()."
             << endl;
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < Piece::PIECE_KIND; k++) {
        if (pl[k] > 0) {
            for (int i = 0; i < int(la.size()); i++) {
                if (la[i].get_piece().get_kind() == k) {
                    new_la.push_back(la[i]);
                }
            }
        }
    }

    return new_la;
}

void Game::shift_player() { current_player = get_next_player(); }

bool Game::set_stone(const Action act) {
    if (board.set_stone(act, player_to_color(current_player), turn)) {
        was_passed = false;

        int kind = act.get_piece().get_kind();
        if (current_player == Player::BLACK) {
            piece_left[kind].first--;
        } else if (current_player == Player::WHITE) {
            piece_left[kind].second--;
        }

        shift_player();
        update_top_view();
        turn++;
        return true;
    }

    return false;
}

void Game::pass_moving() {
    if (was_passed) {
        return end_game(false);
    }

    was_passed = true;
    shift_player();
}

void Game::end_game(bool connected = false) {
    if (connected) {
        Color c = is_connected();
        winner = color_to_player(c);
    } else {
        pair<int, int> pieces = count_pieces();

        int black = pieces.first;
        int white = pieces.second;

        if (black > white) {
            winner = Player::BLACK;
        } else if (black < white) {
            winner = Player::WHITE;
        } else {
            winner = Player::DRAW;
        }
    }
}

bool Game::is_win(const Player player) const { return winner == player; }

bool Game::is_lose(const Player player) const {
    return !is_draw() && winner != player;
}

bool Game::is_draw() const { return winner == Player::DRAW; }

bool Game::is_finished() const { return winner != Player::NONE; }

bool Game::is_start_player() const { return current_player == START_PLAYER; }

Color Game::is_connected() const {
    // 上から見た時の3次元盤面top_viewに対して、盤面の周囲から探索を行い、盤面の対岸にたどり着くことができれば、橋渡しができたとみなし、targetを返す。
    // 橋渡しができていない場合、Color::NONEを返す。
    // 探索開始地点は盤面の周囲からである。

    const int X = 0, Y = 0;

    // Y = 0 のマスを出発点とした探索
    for (int cx = 0; cx < Board::BOARD_SIZE; cx++) {
        if (top_view[Y][cx] != Color::EMPTY) {
            Color target = top_view[Y][cx];

            bool accessed[Board::BOARD_SIZE][Board::BOARD_SIZE];
            for (int y = 0; y < Board::BOARD_SIZE; y++) {
                for (int x = 0; x < Board::BOARD_SIZE; x++) {
                    accessed[y][x] = false;
                }
            }

            if (explore_next(cx, Y, accessed, DIRECTION::Y)) {
                return target;
            }
        }
    }

    // X = 0 のマスを出発点とした探索
    for (int cy = 0; cy < Board::BOARD_SIZE; cy++) {
        if (top_view[cy][X] != Color::EMPTY) {
            Color target = top_view[cy][X];

            bool accessed[Board::BOARD_SIZE][Board::BOARD_SIZE];
            for (int y = 0; y < Board::BOARD_SIZE; y++) {
                for (int x = 0; x < Board::BOARD_SIZE; x++) {
                    accessed[y][x] = false;
                }
            }

            if (explore_next(X, cy, accessed, DIRECTION::X)) {
                return target;
            }
        }
    }
    return Color::NONE;
}

int Game::get_piece_left(const int kind, const Player player) const {
    if (player == Player::BLACK) {
        return this->piece_left[kind].first;
    } else if (player == Player::WHITE) {
        return this->piece_left[kind].second;
    } else {
        cout << "error: Illegal argument in Game::get_piece_left()." << endl;
        exit(EXIT_FAILURE);
    }
}

Player Game::get_cp() const { return current_player; }

Player Game::get_next_player() const {
    return (current_player == Player::BLACK) ? Player::WHITE : Player::BLACK;
}

Player Game::get_winner() const { return winner; }

const Board &Game::get_board() const { return board; }
int Game::get_turn() const { return turn; }

Color Game::get_top_view_cell(int x, int y) const { return top_view[y][x]; }

bool Game::get_was_passed() const { return was_passed; }

void Game::show_all() const {
    show_board();
    show_top_view();
    show_player_status();
}
void Game::show_action(const Action act) const {
    cout << "#" << turn << " << ";

    if (current_player == Player::BLACK) {
        cout << "BLACK ";
    } else if (current_player == Player::WHITE) {
        cout << "WHITE ";
    }

    cout << act.get_string() << " >> " << endl;
}
void Game::show_board() const { board.show_all_board(true); }
void Game::show_top_view() const {
    cout << endl;
    for (int i = 0; i < 9; i++) {
        cout << "**";
    }
    cout << endl;
    cout << " << top view >>" << endl;
    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        cout << " ";
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            if (top_view[y][x] == Color::BLACK) {
                cout << "b ";
            }
            if (top_view[y][x] == Color::WHITE) {
                cout << "w ";
            }
            if (top_view[y][x] == Color::EMPTY) {
                cout << "* ";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < 9; i++) {
        cout << "**";
    }
    cout << endl << endl;
}
void Game::show_player_status() const {
    pair<int, int> pieces = count_pieces();

    cout << "<< Black: ";
    for (int i = 0; i < Piece::PIECE_KIND; i++) {
        cout << piece_left[i].first;
    }
    cout << " (" << pieces.first;
    cout << ") >>  << White: ";
    for (int i = 0; i < Piece::PIECE_KIND; i++) {
        cout << piece_left[i].second;
    }
    cout << " (" << pieces.second;
    cout << ") >>" << endl;
}

Player Game::color_to_player(Color color) const {
    if (color == Color::BLACK) {
        return Player::BLACK;
    }
    if (color == Color::WHITE) {
        return Player::WHITE;
    }

    cout << "error: Illegal argument in <color_to_player>" << endl;
    return Player::NONE;
}

Color Game::player_to_color(Player player) const {
    if (player == Player::BLACK) {
        return Color::BLACK;
    }
    if (player == Player::WHITE) {
        return Color::WHITE;
    }

    cout << "error: Illegal argument in <player_to_color>" << endl;
    return Color::NONE;
}

pair<int, int> Game::count_pieces() const {
    int black = 0, white = 0;
    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            if (top_view[y][x] == Color::BLACK) {
                black++;
            } else if (top_view[y][x] == Color::WHITE) {
                white++;
            }
        }
    }

    return pair<int, int>(black, white);
}

void Game::update_top_view() {
    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            for (int z = Board::BOARD_HEIGHT - 1; z >= 0; z--) {
                if (board.get_color(x, y, z) != Color::EMPTY) {
                    top_view[y][x] = board.get_color(x, y, z);
                    break;
                }

                if (z == 0) {
                    top_view[y][x] = board.get_color(x, y, 0);
                }
            }
        }
    }
}
void Game::init_top_view() { update_top_view(); }

void Game::init_piece_left() {
    for (int k = 0; k < Piece::PIECE_KIND; k++) {
        if (k == 1) {
            piece_left[k].first = 2;
            piece_left[k].second = 2;
        } else {
            piece_left[k].first = 4;
            piece_left[k].second = 4;
        }
    }
}

bool Game::explore_next(const int base_x, const int base_y,
                        bool accessed[][Board::BOARD_SIZE],
                        const DIRECTION D) const {
    // 隣接するマスについて調べ、それらが対象の色であれば、そのマスの隣接マスについてこの関数を呼び出す(再帰関数)。
    accessed[base_y][base_x] = true;

    if (D == DIRECTION::X) {
        if (base_x == Board::BOARD_SIZE - 1) {
            return true;
        }
    } else {
        if (base_y == Board::BOARD_SIZE - 1) {
            return true;
        }
    }

    Color target = top_view[base_y][base_x];

    const int SS_SIZE = 4;
    pair<int, int> ss[SS_SIZE];

    int ss_i = 0;

    // x方向の隣接マスについて調べる
    for (int dx = -1; dx <= 1; dx++) {
        if (dx == 0) {
            continue;
        }

        int rx = base_x + dx;
        if (rx < 0 || Board::BOARD_SIZE <= rx) {
            ss[ss_i++] = pair<int, int>(0, 0);
            continue;
        }
        int ry = base_y;

        if (accessed[ry][rx]) {
            ss[ss_i++] = pair<int, int>(0, 0);
        }

        else if (top_view[ry][rx] == target) {
            ss[ss_i++] = pair<int, int>(dx, 0);
        } else {
            ss[ss_i++] = pair<int, int>(0, 0);
        }
    }
    // y方向の隣接マスについて調べる
    for (int dy = -1; dy <= 1; dy++) {
        if (dy == 0) {
            continue;
        }

        int rx = base_x;
        int ry = base_y + dy;

        if (ry < 0 || Board::BOARD_SIZE <= ry) {
            ss[ss_i++] = pair<int, int>(0, 0);
            continue;
        }

        if (accessed[ry][rx]) {
            ss[ss_i++] = pair<int, int>(0, 0);
        }

        else if (top_view[ry][rx] == target) {
            ss[ss_i++] = pair<int, int>(0, dy);
        } else {
            ss[ss_i++] = pair<int, int>(0, 0);
        }
    }

    for (int i = 0; i < SS_SIZE; i++) {
        int dx = ss[i].first;
        int dy = ss[i].second;

        if (dx == 0 && dy == 0) {
            continue;
        }

        int rx = base_x + dx;
        int ry = base_y + dy;

        if (explore_next(rx, ry, accessed, D)) {
            return true;
        }
    }
    return false;
}
