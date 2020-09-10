#include "format.h"

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours, min, sec;
  string hours_str, min_str, sec_str;

  hours = floor(seconds / 3600);
  seconds = floor(seconds - (hours * 3600));
  min = floor(seconds / 60);
  sec = seconds - (min * 60);

  hours_str = hours < 10 ? ("0" + to_string(hours)) : to_string(hours);
  min_str = min < 10 ? ("0" + to_string(min)) : to_string(min);
  sec_str = sec < 10 ? ("0" + to_string(sec)) : to_string(sec);

  return hours_str + ":" + min_str + ":" + sec_str;
}

// Helper function to convert Kilobytes into Megabytes
string Format::ConvertRam(string kilobytesStr) {
  int kilobytes = std::stoi(kilobytesStr);
  int megabytes = (kilobytes / 1024);
  return to_string(megabytes);
}