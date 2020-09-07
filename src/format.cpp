#include "format.h"

#include <string>
#include <cmath>
#include <vector>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

    int hours, min, sec;
    string hours_str, min_str, sec_str;

    hours = floor(seconds / 3600);
    seconds = floor(seconds - (hours*3600));
    min = floor(seconds / 60);
    sec = seconds - (min * 60);

    hours_str = hours < 10 ? ("0" + to_string(hours)) : to_string(hours);
    min_str = min < 10 ? ("0" + to_string(min)) : to_string(min);
    sec_str = sec < 10 ? ("0" + to_string(sec)) : to_string(sec);

    return hours_str + ":" + min_str + ":" + sec_str;
    }