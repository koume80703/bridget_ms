#include <cell.hpp>

Cell::Cell() {}

Cell::Cell(int index = 0, Color color = Color::EMPTY) {
    this->index = index;
    this->color = color;
}

Cell::Cell(const Cell &rhs) {
    this->index = rhs.index;
    this->color = rhs.color;
}

Cell &Cell::operator=(const Cell &rhs) {
    this->index = rhs.index;
    this->color = rhs.color;

    return *this;
}

Color Cell::get_color() const { return this->color; }

int Cell::get_index() const { return this->index; }