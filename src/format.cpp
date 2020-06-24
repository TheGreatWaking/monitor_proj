#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hh = seconds / 3600;
    int mm = (seconds - hh*3600) / 60;
    int ss = seconds - hh*3600 - mm*60;
    return std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss); 
}