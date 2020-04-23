#ifndef PASSENGER_H //Guard for header file.
#define PASSENGER_H

struct Passenger{   //Passenger struct
    int passenger_id;
    char passenger_type;
    char pass_type;
    bool* monthlyPasses;
    bool isTWorker;
    double pass_balance;
};


#endif