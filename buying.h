#ifndef BUYING_H_ //Guard for header file.
#define BUYING_H_

void choiceError(char* userChoice, long* choice, size_t size, int errorType); //This method will be use to check any errors in input and ensure that a correct input is given
bool securityCheck(); //This method is a security check to make sure that the person is actually a transportation worker using a password
void monthly(char rider_type, double* r_prices, double* d_prices, bool* pass); //This function is used to buy monthly tickets
void per_pass(char rider_type); //This function is used to buy pay-per-ride tickets
void buy_pass(struct Passenger *user);
int get_id();
double pay_per_ride();


#endif