#include <action.hpp>

Action::Action() {}

Action::Action(int x, int y, Piece piece) {
    this->xy = pair<int, int>(x, y);
    this->piece = piece;
}

Action::Action(const Action& rhs) {
    this->xy = rhs.xy;
    this->piece = rhs.piece;
}

Action& Action::operator=(const Action& rhs) {
    this->xy = rhs.xy;
    this->piece = rhs.piece;

    return *this;
}

bool Action::operator==(const Action& rhs) const {
    return this->xy == rhs.xy && this->piece == rhs.piece;
}

pair<int, int> Action::get_xy() const { return this->xy; }

const Piece& Action::get_piece() const { return this->piece; }

string Action::get_string() const {
    string str = "#";

    int x = this->xy.first;
    int y = this->xy.second;

    str += std::to_string(x);
    str += std::to_string(y);

    str += "# ";

    str += this->piece.get_string();

    return str;
}