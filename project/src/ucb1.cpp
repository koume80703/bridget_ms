#include <ucb1.hpp>

double ucb1(const int sn, const int n, const int w) {
    return double(w) / double(n) + sqrt((2 * std::log(sn)) / double(n));
}