#include <board.hpp>

Board::Board() {
    for (int z = 0; z < BOARD_HEIGHT; z++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                SolidBoard[z][y][x] = Cell(0, Color::EMPTY);
            }
        }
    }
}

Board::Board(const Board &rhs) {
    for (int z = 0; z < BOARD_HEIGHT; z++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                SolidBoard[z][y][x] = rhs.SolidBoard[z][y][x];
            }
        }
    }
}

Board &Board::operator=(const Board &rhs) {
    for (int z = 0; z < BOARD_HEIGHT; z++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                SolidBoard[z][y][x] = rhs.SolidBoard[z][y][x];
            }
        }
    }

    return *this;
}

bool Board::set_stone(const Action act, const Color color, const int turn) {
    if (!placable_cell(act)) {
        cout << "Illegal action. Can not set stone." << endl;
        return false;
    }

    tuple<int, int, int> abs_c[ABS_C_SIZE];
    get_abs_coordinate(abs_c, act);

    Cell c = Cell(turn, color);

    for (int i = 0; i < ABS_C_SIZE; i++) {
        int x = std::get<XC>(abs_c[i]);
        int y = std::get<YC>(abs_c[i]);
        int z = std::get<ZC>(abs_c[i]);

        SolidBoard[z][y][x] = c;
    }

    return true;
}

vector<Action> Board::legal_actions() const {
    vector<Action> la;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int k = 0; k < Piece::PIECE_KIND; k++) {
                for (int rdy = 0; rdy < 4; rdy++) {
                    for (int rdx = 0; rdx < 4; rdx++) {
                        Piece p = Piece(k, rdx, rdy);
                        Action act = Action(x, y, p);

                        if (DEBUG_BOARD) {
                            cout << act.get_string() << endl;
                        }

                        if (placable_cell(act)) {
                            la.push_back(act);
                        }
                    }
                }
            }
        }
    }

    return la;
}

bool Board::placable_cell(const Action act) const {
    /*
    Actionクラスから得られる情報は駒pieceの基準小体を座標xyに配置するということである。このActionが意味するのは、基準小体をxy上で下に空間を持たないように配置するということである。
    この関数で求めるべきはこのようにして基準小体を配置した時に駒全体の配置が合法手であるかどうかということである。
     */
    /*
    bridgetにおいて配置可能マスとは、
    ・駒の小体のどれかが盤面に接地していること
    ・駒の小体の全てについて、1つ下のマスに空間を持たないこと
    ・すでに小体のあるマスに配置したい駒の小体が重ならないこと
    ・盤面外に小体がはみ出ることがないこと。盤面内に駒が収まっていること。（立体なのでz=-1,3にはみ出ることがないように考える。）
    ・同じ色の駒の小体に配置する駒の小体が接していること
    これらを満たす必要がある。
    */

    /*
    Pieceクラスによって駒の種類及び向きが一意に定まり、
    その駒が座標(x,y)に配置可能であればtrueを返すような関数を作成する。
    */
    if (!is_inside(act)) {
        if (DEBUG_BOARD) {
            cout << "is NOT inside." << endl;
        }
        return false;
    }
    if (!is_on_ground(act)) {
        if (DEBUG_BOARD) {
            cout << "is NOT on ground." << endl;
        }
        return false;
    }
    if (is_having_space(act)) {
        if (DEBUG_BOARD) {
            cout << "is having space." << endl;
        }
        return false;
    }
    if (is_duplicated(act)) {
        if (DEBUG_BOARD) {
            cout << "is duplicated." << endl;
        }
        return false;
    }
    if (DEBUG_BOARD) {
        cout << "discovered legal action" << endl;
    }

    return true;
}

void Board::get_abs_coordinate(tuple<int, int, int> *abs_c,
                               const Action act) const {
    abs_c[0] = get_base_block(act);
    const Piece p = act.get_piece();

    const int RC_SIZE = 3;
    tuple<int, int, int> rcs[RC_SIZE];
    p.get_rcs(rcs);

    for (int i = 1; i < ABS_C_SIZE; i++) {
        int abs_x, abs_y, abs_z;
        const int rc_index = i - 1;

        abs_x = std::get<XC>(abs_c[0]) + std::get<XC>(rcs[rc_index]);
        abs_y = std::get<YC>(abs_c[0]) + std::get<YC>(rcs[rc_index]);
        abs_z = std::get<ZC>(abs_c[0]) + std::get<ZC>(rcs[rc_index]);

        abs_c[i] = tuple<int, int, int>(abs_x, abs_y, abs_z);
    }
}

tuple<int, int, int> Board::get_base_block(const Action act) const {
    // actによって決められるのは、基準小体のxy座標のみであるので、z座標は別に計算する必要がある。
    // よって、この関数にてz座標を求め、3次元盤面上で基準小体の位置(x,y,z)を返す。
    // あるxy座標におけるどのz座標について、すでに何らかの駒が存在する場合、このxy座標上には駒を配置することができない。
    // よって、不正なActionであるということでz座標に-1を入れて返すようにした。
    // z座標が-1の場合、駒全体が盤面外に配置されることになるので、合法手になり得ない。

    const int x = act.get_xy().first;
    const int y = act.get_xy().second;

    Piece p = act.get_piece();
    tuple<int, int, int> rcs[Piece::RC_SIZE];
    p.get_rcs(rcs);

    for (int z = 0; z < BOARD_HEIGHT; z++) {
        // 全ての小体のz座標0を越えるように基準小体のz座標を定める。
        for (int i = 0; i < Piece::RC_SIZE; i++) {
            if (std::get<ZC>(rcs[i]) + z < 0) {
                break;
            }
            if (i == Piece::RC_SIZE - 1) {
                return tuple<int, int, int>(x, y, z);
            }
        }
    }
    return tuple<int, int, int>(x, y, -1);
}

bool Board::is_on_ground(const Action act) const {
    // 配置する駒について各小体のどれかが地面に接地しているか判定する関数。
    // 仮配置した時に、各小体のある座標を計算し、そのz座標が1つでも0なら地面に接地していることになるので、trueを返す

    tuple<int, int, int> abs_c[ABS_C_SIZE];
    get_abs_coordinate(abs_c, act);

    for (int i = 0; i < ABS_C_SIZE; i++) {
        if (std::get<ZC>(abs_c[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool Board::is_having_space(const Action act) const {
    // 各小体の1つ下のマスに空間があるのかどうか判定する関数、
    // 仮配置した時に、各小体のある座標を計算し、そのz座標を-1したものが1つ下のマスに当たるので、そのマスについて調べ、何かしらの駒が存在することが確認する。EMPTYならtrueを返す(戻り値は関数名に従う)。
    // あくまで、仮配置をこの関数内で行っているだけなので、盤面情報は更新されていない。
    // ある小体について調べた時に、その正体の下のマスがEMPTYだったとしても駒の持つ他の小体がその下のマスにある場合、それは空間を持つとは言えない。

    tuple<int, int, int> abs_c[ABS_C_SIZE];
    get_abs_coordinate(abs_c, act);

    bool ss[ABS_C_SIZE];
    for (int i = 0; i < ABS_C_SIZE; i++) {
        ss[i] = true;
    }

    for (int i = 0; i < ABS_C_SIZE; i++) {
        const int x = std::get<XC>(abs_c[i]);
        const int y = std::get<YC>(abs_c[i]);
        const int z = std::get<ZC>(abs_c[i]);

        for (int j = 0; j < ABS_C_SIZE; j++) {
            if (i == j) {
                continue;
            }

            if (x == std::get<XC>(abs_c[j]) && y == std::get<YC>(abs_c[j]) &&
                z - 1 == std::get<ZC>(abs_c[j])) {
                ss[i] = false;
                break;
            }
        }
    }

    for (int i = 0; i < ABS_C_SIZE; i++) {
        const int x = std::get<XC>(abs_c[i]);
        const int y = std::get<YC>(abs_c[i]);
        const int z = std::get<ZC>(abs_c[i]);

        if (z - 1 < 0) {
            continue;
        }

        if (ss[i]) {
            const Cell c = SolidBoard[z - 1][y][x];

            if (c.get_color() == Color::EMPTY) {
                return true;
            }
        }
    }

    return false;
}

bool Board::is_duplicated(const Action act) const {
    // 各小体がすでに盤面内にある駒と重複せずに配置できているか判定する関数。
    // 仮配置した時に、各小体のある座標を計算し、その座標上に駒がないことを判定する。

    tuple<int, int, int> abs_c[ABS_C_SIZE];
    get_abs_coordinate(abs_c, act);

    for (int i = 0; i < ABS_C_SIZE; i++) {
        const int x = std::get<XC>(abs_c[i]);
        const int y = std::get<YC>(abs_c[i]);
        const int z = std::get<ZC>(abs_c[i]);

        const Cell c = SolidBoard[z][y][x];

        if (c.get_color() != Color::EMPTY) {
            return true;
        }
    }
    return false;
}

bool Board::is_inside(const Action act) const {
    // 駒全体が立体盤面内に収まっているか判定する関数。
    // 仮配置した時に、各小体のある座標を計算し、それが立体盤面内であるかif文を元に判定すれば良い。

    tuple<int, int, int> abs_c[ABS_C_SIZE];
    get_abs_coordinate(abs_c, act);

    for (int i = 0; i < ABS_C_SIZE; i++) {
        const int x = std::get<XC>(abs_c[i]);
        const int y = std::get<YC>(abs_c[i]);
        const int z = std::get<ZC>(abs_c[i]);

        if (!(0 <= x && x < BOARD_SIZE)) {
            return false;
        }
        if (!(0 <= y && y < BOARD_SIZE)) {
            return false;
        }
        if (!(0 <= z && z < BOARD_HEIGHT)) {
            return false;
        }
    }
    return true;
}

void Board::show_all_board(const bool horizontal) const {
    if (horizontal) {
        show_board_horizontal();
    } else {
        for (int i = 0; i < 10; i++) {
            cout << "**";
        }
        cout << endl;
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            show_board_1f(i);
        }
        for (int i = 0; i < 10; i++) {
            cout << "**";
        }
        cout << endl;
    }
}
void Board::show_board_1f(const int z) const {
    cout << "<< floor: " << z << " >>" << endl;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (SolidBoard[z][y][x].get_color() == Color::BLACK) {
                cout << "B ";
            } else if (SolidBoard[z][y][x].get_color() == Color::WHITE) {
                cout << "W ";
            } else if (SolidBoard[z][y][x].get_color() == Color::EMPTY) {
                cout << "* ";
            } else {
                cout << "Illegal cell information in Solid Board" << endl;
                break;
            }
        }
        cout << endl;
    }
}

void Board::show_board_horizontal() const {
    for (int i = 0; i < 53; i++) {
        cout << "*";
    }
    cout << endl;

    for (int z = 0; z < BOARD_HEIGHT; z++) {
        cout << "<< floor:  " << z << " >>    ";
    }
    cout << endl;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int z = 0; z < BOARD_HEIGHT; z++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (SolidBoard[z][y][x].get_color() == Color::BLACK) {
                    cout << "B ";
                } else if (SolidBoard[z][y][x].get_color() == Color::WHITE) {
                    cout << "W ";
                } else if (SolidBoard[z][y][x].get_color() == Color::EMPTY) {
                    cout << "* ";
                } else {
                    cout << "Illegal cell information in Solid Board" << endl;
                    break;
                }
            }
            cout << "   ";
        }
        cout << endl;
    }

    for (int i = 0; i < 53; i++) {
        cout << "*";
    }
    cout << endl << endl;
}

Color Board::get_color(const int x, const int y, const int z) const {
    return SolidBoard[z][y][x].get_color();
}

int Board::get_index(const int x, const int y, const int z) const {
    return SolidBoard[z][y][x].get_index();
}
