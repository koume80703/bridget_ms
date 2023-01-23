#include <piece.hpp>

Piece::Piece() {}

Piece::Piece(int kind, int rdx, int rdy) {
    this->kind = kind;
    this->rdx = rdx;
    this->rdy = rdy;

    init_rcs(this->rcs);
}

Piece::Piece(const Piece &rhs) {
    this->kind = rhs.kind;
    this->rdx = rhs.rdx;
    this->rdy = rhs.rdy;

    for (int i = 0; i < RC_SIZE; i++) {
        this->rcs[i] = rhs.rcs[i];
    }
}

Piece &Piece::operator=(const Piece &rhs) {
    this->kind = rhs.kind;
    this->rdx = rhs.rdx;
    this->rdy = rhs.rdy;

    for (int i = 0; i < RC_SIZE; i++) {
        this->rcs[i] = rhs.rcs[i];
    }

    return *this;
}

bool Piece::operator==(const Piece &rhs) const {
    if (this->kind != rhs.kind) {
        return false;
    }
    if (this->rdx != rhs.rdx) {
        return false;
    }
    if (this->rdy != rhs.rdy) {
        return false;
    }
    for (int i = 0; i < RC_SIZE; i++) {
        if (this->rcs[i] != rhs.rcs[i]) {
            return false;
        }
    }

    return true;
}

string Piece::str_rcs(int z) const {
    string s_rcs = "(";
    s_rcs += to_string(std::get<XC>(this->rcs[z])) + ", ";
    s_rcs += to_string(std::get<YC>(this->rcs[z])) + ", ";
    s_rcs += to_string(std::get<ZC>(this->rcs[z]));
    s_rcs += ")";

    return s_rcs;
}

int Piece::get_kind() const { return this->kind; }

int Piece::get_rdx() const { return this->rdx; }

int Piece::get_rdy() const { return this->rdy; }

void Piece::get_rcs(tuple<int, int, int> *dst_rcs) const {
    for (int i = 0; i < RC_SIZE; i++) {
        int x, y, z;
        x = std::get<XC>(this->rcs[i]);
        y = std::get<YC>(this->rcs[i]);
        z = std::get<ZC>(this->rcs[i]);
        dst_rcs[i] = tuple<int, int, int>(x, y, z);
    }
}

string Piece::get_string() const {
    string str = "";
    if (this->kind == 0) {
        str += "L";
    } else if (this->kind == 1) {
        str += "O";
    } else if (this->kind == 2) {
        str += "S";
    } else if (this->kind == 3) {
        str += "T";
    }

    str += std::to_string(this->rdx);
    str += std::to_string(this->rdy);

    str += " ";

    str += str_rcs(0);
    str += str_rcs(1);
    str += str_rcs(2);

    return str;
}

void Piece::init_rcs(tuple<int, int, int> *rcs) {
    // 基準小体の座標を(0,0,0)としたときの、その他3つの小体の座標を引数によって与えられた長さ3の配列rcsに格納することで初期化する。
    // 座標系はxy平面においては左上を原点として、右へ進むとxが1増え、下へ進むとyが1増えるような座標系である。
    // z座標については、上に進むとzが1増えるような座標となっている。
    // tuple<int,int,int>(x, y, z)としている。

    if (this->kind == 0) {
        if (this->rdy == 0) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 2, 0);
                rcs[2] = tuple<int, int, int>(1, 2, 0);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(-1, 0, 0);
                rcs[1] = tuple<int, int, int>(-2, 0, 0);
                rcs[2] = tuple<int, int, int>(-2, 1, 0);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, -1, 0);
                rcs[1] = tuple<int, int, int>(0, -2, 0);
                rcs[2] = tuple<int, int, int>(-1, -2, 0);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(2, 0, 0);
                rcs[2] = tuple<int, int, int>(2, -1, 0);
            }
        } else if (this->rdy == 1) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(0, 0, -2);
                rcs[2] = tuple<int, int, int>(1, 0, -2);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(0, 0, -2);
                rcs[2] = tuple<int, int, int>(0, 1, -2);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(0, 0, -2);
                rcs[2] = tuple<int, int, int>(-1, 0, -2);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(0, 0, -2);
                rcs[2] = tuple<int, int, int>(0, -1, -2);
            }
        } else if (this->rdy == 2) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, -1, 0);
                rcs[1] = tuple<int, int, int>(0, -2, 0);
                rcs[2] = tuple<int, int, int>(1, -2, 0);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(2, 0, 0);
                rcs[2] = tuple<int, int, int>(2, 1, 0);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 2, 0);
                rcs[2] = tuple<int, int, int>(-1, 2, 0);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(-1, 0, 0);
                rcs[1] = tuple<int, int, int>(-2, 0, 0);
                rcs[2] = tuple<int, int, int>(-2, -1, 0);
            }
        } else if (this->rdy == 3) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(0, 0, 2);
                rcs[2] = tuple<int, int, int>(1, 0, 2);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(0, 0, 2);
                rcs[2] = tuple<int, int, int>(0, 1, 2);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(0, 0, 2);
                rcs[2] = tuple<int, int, int>(-1, 0, 2);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(0, 0, 2);
                rcs[2] = tuple<int, int, int>(0, -1, 2);
            }
        }
    } else if (this->kind == 1) {
        if (this->rdy % 2 == 0) {
            rcs[0] = tuple<int, int, int>(0, 1, 0);
            rcs[1] = tuple<int, int, int>(1, 0, 0);
            rcs[2] = tuple<int, int, int>(1, 1, 0);
        } else if (this->rdy % 2 == 1) {
            if (this->rdx % 2 == 0) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(0, 0, -1);
                rcs[2] = tuple<int, int, int>(1, 0, -1);
            } else if (this->rdx % 2 == 1) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 1, -1);
                rcs[2] = tuple<int, int, int>(0, 0, -1);
            }
        }
    } else if (this->kind == 2) {
        if (this->rdy == 0) {
            if (this->rdx % 2 == 0) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(1, 1, 0);
                rcs[2] = tuple<int, int, int>(1, 2, 0);
            } else if (this->rdx % 2 == 1) {
                rcs[0] = tuple<int, int, int>(-1, 0, 0);
                rcs[1] = tuple<int, int, int>(-1, -1, 0);
                rcs[2] = tuple<int, int, int>(-2, -1, 0);
            }
        } else if (this->rdy == 1) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(1, 0, -1);
                rcs[2] = tuple<int, int, int>(1, 0, -2);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(0, 1, -1);
                rcs[2] = tuple<int, int, int>(0, 1, -2);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(-1, 0, -1);
                rcs[2] = tuple<int, int, int>(-1, 0, -2);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(0, 0, -1);
                rcs[1] = tuple<int, int, int>(0, -1, -1);
                rcs[2] = tuple<int, int, int>(0, -1, -2);
            }
        } else if (this->rdy == 2) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, -1, 0);
                rcs[1] = tuple<int, int, int>(1, -1, 0);
                rcs[2] = tuple<int, int, int>(1, -2, 0);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(1, 1, 0);
                rcs[2] = tuple<int, int, int>(2, 1, 0);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(-1, 1, 0);
                rcs[2] = tuple<int, int, int>(-1, 2, 0);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(1, 1, 0);
                rcs[2] = tuple<int, int, int>(2, 1, 0);
            }
        } else if (this->rdy == 3) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(1, 0, 1);
                rcs[2] = tuple<int, int, int>(1, 0, 2);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(0, 1, 1);
                rcs[2] = tuple<int, int, int>(0, 1, 2);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(-1, 0, 1);
                rcs[2] = tuple<int, int, int>(-1, 0, 2);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(0, 0, 1);
                rcs[1] = tuple<int, int, int>(0, -1, 1);
                rcs[2] = tuple<int, int, int>(0, -1, 2);
            }
        }
    } else if (this->kind == 3) {
        if (this->rdy == 0) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(1, 1, 0);
                rcs[2] = tuple<int, int, int>(2, 0, 0);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 2, 0);
                rcs[2] = tuple<int, int, int>(-1, 1, 0);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(-1, 0, 0);
                rcs[1] = tuple<int, int, int>(-2, 0, 0);
                rcs[2] = tuple<int, int, int>(-1, -1, 0);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(0, -1, 0);
                rcs[1] = tuple<int, int, int>(0, -2, 0);
                rcs[2] = tuple<int, int, int>(1, -1, 0);
            }
        } else if (this->rdy == 1) {
            if (this->rdx % 2 == 0) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(2, 0, 0);
                rcs[2] = tuple<int, int, int>(1, 0, -1);
            } else if (this->rdx % 2 == 1) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 2, 0);
                rcs[2] = tuple<int, int, int>(0, 1, -1);
            }
        } else if (this->rdy == 2) {
            if (this->rdx == 0) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(2, 0, 0);
                rcs[2] = tuple<int, int, int>(1, -1, 0);
            } else if (this->rdx == 1) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 2, 0);
                rcs[2] = tuple<int, int, int>(1, 1, 0);
            } else if (this->rdx == 2) {
                rcs[0] = tuple<int, int, int>(-1, 0, 0);
                rcs[1] = tuple<int, int, int>(-2, 0, 0);
                rcs[2] = tuple<int, int, int>(-1, 1, 0);
            } else if (this->rdx == 3) {
                rcs[0] = tuple<int, int, int>(0, -1, 0);
                rcs[1] = tuple<int, int, int>(0, -2, 0);
                rcs[2] = tuple<int, int, int>(-1, -1, 0);
            }
        } else if (this->rdy == 3) {
            if (this->rdx % 2 == 0) {
                rcs[0] = tuple<int, int, int>(1, 0, 0);
                rcs[1] = tuple<int, int, int>(2, 0, 0);
                rcs[2] = tuple<int, int, int>(1, 0, 1);
            } else if (this->rdx % 2 == 1) {
                rcs[0] = tuple<int, int, int>(0, 1, 0);
                rcs[1] = tuple<int, int, int>(0, 2, 0);
                rcs[2] = tuple<int, int, int>(0, 1, 1);
            }
        }
    }
}