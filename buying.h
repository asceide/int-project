#ifndef BUYING_H //Guard for header file.
#define BUYING_H

void choiceError(char* userChoice, long* e_choice, size_t size, int errorType); //This method will be use to check any errors in input and ensure that a correct input is given
bool securityCheck(); //This method is a security check to make sure that the person is actually a transportation worker using a password
void monthly(char rider_type, double* r_prices, double* d_prices, bool* pass); //This function is used to buy monthly tickets. Since the user at this point may have not bought a pass, I am not sending in the Passenger struct as a parameter
void per_pass(char rider_type); //This function is used to buy pay-per-ride tickets
void buy_pass(struct Passenger *user); //this function is used to buy pass. Since all this does is add money to the pass I am fine to send in the Passenger struct
int get_u_id(); //This function generates a random id number. the ID number is not necessary at this time, however it may be in the future so I set it up now just in case.
double pay_per_ride(); //Gets the user money.

#endif