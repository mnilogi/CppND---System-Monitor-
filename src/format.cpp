#include <string>
#include <stdlib.h>
#include "format.h"
#include <cstring>
using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
     int second = 0;
     int minuete = 0;
     int hour = 0;
     second = seconds % 60;
     minuete = seconds / 60;//total number of whole minuetes
     minuete %= 60;
     hour = seconds / 3600;
     std::string out;
     std::string zero{},zero1{},zero2{};
     if(second < 10)
          zero2 = '0';
     if(minuete < 10)
          zero1 = '0';
     if(hour < 10)
          zero = '0';
     out = zero + std::to_string(hour) + ':' + zero1 + std::to_string(minuete) + ':' + zero2 + std::to_string(second); 
     return out;
     }
