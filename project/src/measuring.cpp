#include "measuring.hpp"

double evaluate_time, total_evaluate_time;
double playout_time, total_playout_time;
double la_time, total_la_time;
double ucb_time, total_ucb_time;
double expand_time, total_expand_time;

int playout_count, evaluate_count, la_count, expand_count, ucb_count;

double start, end;

double get_time_msec() {
    return static_cast<double>(duration_cast<nanoseconds>(
                                   steady_clock::now().time_since_epoch())
                                   .count()) /
           (1000 * 1000);
}

void print_time(double time, string str) {
    printf("%-30s: %.2f [ms]\n", str.c_str(), time);
}

void print_percentage(double value, string kind) {
    printf("%-30s: %.3f [%%]\n", ("Percentage " + kind).c_str(), value * 100);
}
