#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>

#include "parameter.hpp"

using namespace std;
using namespace std::chrono;

const bool TIME_MEASURING = measuring::TIME_MEASURING;

double get_time_msec();

void print_time(double time, string str = "");

void print_percentage(double value, string kind = "");
