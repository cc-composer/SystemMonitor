#include <string>

#include "format.h"
#include "linux_parser.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    string HH, MM, SS = "";
    long hours, minutes;

    hours = seconds/3600;
    minutes = (seconds/60) - (hours*60);
    seconds = seconds - (minutes*60) - (hours*3600);
    
    HH = hours >= 10 ? to_string(hours) : "0" + to_string(hours);
    MM = minutes >= 10 ? to_string(minutes) : "0" + to_string(minutes);
    SS = seconds >= 10 ? to_string(seconds) : "0" + to_string(seconds);
    
    return HH + ":" + MM + ":" + SS; 
}