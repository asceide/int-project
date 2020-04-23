#ifndef USING_H //Guard for header file.
#define USING_H


void using_choice_error(char* userChoice, long* choice, size_t size, int errorType, double* prices);
void monthly_usage(struct Passenger *user);
void pay_per_usage(struct Passenger *user);
void use_pass(struct Passenger *user);

#endif