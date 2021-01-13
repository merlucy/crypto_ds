#ifndef D_STRUCT_H
#define D_STRUCT_H

#include <string>

// struct to store historical data point
typedef struct _hist_p{
    double close;
    double high;
    double low;
    double open;
    std::string time;
    double epoch;
    double volume;

}hist_p;

#endif