/*  Author: Patrick Alarcon
*   This module is used for the Use Pass option of the main menu. As stated, if the user is a transportation worker they will just return to the main menu as there is no need for them use any options. Otherwise
*   If they select monthly, the user is checked to see if their monthly pass is active. If they select Pay-per-usage, they will be shown a list of prices and then be asked to buy a pass. Afterwards the pass is deducted from their balance and they're good to go.
*/

#include <stdlib.h>//Standard included libraries
#include <stdio.h>
#include <time.h>//Library is included for getting current time 
#include <stdbool.h>
#include <string.h>
#include "passenger.h"//Passenger struct
#include "loadprices.h"//Using the loadprices module in order to get the prices of rides, discounts, price per stop, etc.
#ifdef _WIN32 //In order to clear the screen instead of relying on system calls as that isn't compliant.
#include <conio.h>
#else
#define clrscr() printf("\e[1;1H\e[2J") //Using escapes and regex to clear screen on posix/Unix systems.
#endif
//Macros for the various different type of menus
#define USING_MENU "Charlotte Transportation Authority\n\nWhat type of Pass are you using?\n1)Monthly\n2)Pay-per-ride\n3)Return\n"
#define MONTHLY_USAGE_E "What type of transportation will you be using?\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Special Bus\n5)Go back to Menu\n"
#define PAY_USAGE_E "What type of transportation will you be using? PRICES:\nBus: %.2f\nSubway: %.2f (Baseline)\nCommuter Rail: %.2f (Baseline)\nSpecial Bus: %.2f\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Special Bus\n5)Go back to main menu\n"
#define MONTHLY_USAGE_NS "What type of transportation will you be using?\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Go back to Menu\n"
#define PAY_USAGE_NS "What type of transportation will you be using? PRICES:\nBus: %.2f\nSubway: %.2f (Baseline)\nCommuter Rail: %.2f (Baseline)\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Go back to main menu\n"
#define ERROR_WRONG_PASS "Your monthly pass is not eligible for this mode of transportation.\nPlease select a valid mode or buy a monthly pass for this type of transportation.\n"
#define NOT_ENOUGH_MONEY "Your current balance is not enough to use this mode of transportation.\nPlease select another method or place more funds in your pass.\n"
#define INCORRECT_CHOICE "You have selected an incorrect choice.\n"
#define DATE "Todays date is: %s\n\n\n\n"
#define ENJOY_DAY "Enjoy your day!\n(Press Enter to Continue)"
#define TRANSACTION "The cost of the ride is %.2f, and you have %.2f in your card balance.\n"
#define TRAN_ERROR "You do not have enough funds for this ride. Please fund your pass and try again later.\n"
#define AFTER_TRAN "Your updated balance is %.2f\n"
#define PRICE_STOP "Price per stop is currently a flat %.2f. \nHowever, in the future we will be implementing a cost increase for using the subway or commuter rail depending on how many stops you will have to go through. \nPlease keep this in mind in the future.\n"
struct tm *local;//tm struct for getting the day of the week etc.
time_t current;

void using_choice_error(char* userChoice, long* choice, size_t size, int errorType, double* prices); //When a user chooses an incorrect input
void monthly_usage(struct Passenger *user); //If a user selects to use a monthly pass
void pay_per_usage(struct Passenger *user); //If a user selects to pay-per-ride
void use_pass(struct Passenger *user); //The main function of the using option.


void use_pass(struct Passenger *user){
    char* mu_choice=NULL;//For getline
    size_t size_i=1;
    long iu_choice;
    //If the user hasn't bought a pass yet, they're kicked back to the main menu. Since the passenger-id is initialized to -1 and changes when they're buying a pass, this will only happen if they select the using option to start out with
    mu_choice=malloc(size_i*sizeof(char));
    if(user->passenger_id<0){
        printf("You have not bought a pass yet. Please buy a pass first!\n");
        getline(&mu_choice, &size_i, stdin);
        return;
    }
    time(&current); //Gets the current time
    local=localtime(&current); //For getting the day of the week.
    clrscr();
    printf(USING_MENU);//Buying menu.
    getline(&mu_choice, &size_i, stdin);
    iu_choice=atol(mu_choice);//Shows the user their choices. 1 is Monthly passes, 2 is Pay passes, 3 is to return
    if(iu_choice<=0 || iu_choice>3){ //If the user selects anything other than the options listed above.
        using_choice_error(mu_choice, &iu_choice, size_i, 0, NULL);
    }
    if(user->isTWorker==true){
        clrscr();
        printf("As a transportation worker, you're eligible for free transportation. Enjoy!\n");
        getline(&mu_choice, &size_i, stdin);
        return;
    }
    //Since the menu is only going to be run once, there is no need to place it inside a loop.
    switch(iu_choice){
        case 1:
                monthly_usage(user);
                break;
        case 2:
                pay_per_usage(user);
                break;
        case 3:
        default:
                break;
    }
    //Freeing allocated memory
    free(mu_choice);
}
//This is an error checking method. It takes in the user choice from getline, a pointer to the address containing that choice, the second parameter for getline, an integer that will specify the error type, and if needed the prices for the menu. Otherwise NULL is passed.
void using_choice_error(char* userChoice, long* choice, size_t size, int errorType, double* prices){
    switch(errorType){
        case 0: //Error checking the main menu
                while(*choice<=0 || *choice>3){
                    clrscr();
                    printf(INCORRECT_CHOICE);
                    printf(USING_MENU);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
                break;
        case 1: //Error checking Monthly passes menu
                while(*choice<=0 || *choice>4){
                clrscr();
                printf(INCORRECT_CHOICE);
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_NS);
                getline(&userChoice, &size, stdin);
                *choice=atol(userChoice);
                }
                break;
        case 2:
                while(*choice<=0 || *choice>5){
                clrscr();
                printf(INCORRECT_CHOICE);
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_E);
                getline(&userChoice, &size, stdin);
                *choice=atol(userChoice);
                }
                break;
        case 3: //Error checking Pay per ride passes menu
                while(*choice<=0 || *choice>4){
                    clrscr();
                    printf(INCORRECT_CHOICE);
                    printf(DATE, ctime(&current));
                    printf(PAY_USAGE_NS, prices[0], prices[1], prices[2]);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
        case 4:
                while(*choice<=0 || *choice>5){
                    clrscr();
                    printf(INCORRECT_CHOICE);
                    printf(DATE, ctime(&current));
                    printf(PAY_USAGE_E, prices[0], prices[1], prices[2], prices[3]);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
        default: //Otherwise just return to the main menu.
                return;
    }
}
//This is a submenu for monthly usage pass. If the user has an active monthly pass they successfully 'use' it. Otherwise, they're kicked back to the main menu.
void monthly_usage(struct Passenger *user){
    char* m_choice=NULL; //getline usage.
    size_t size_m=1;
    long choice_m;

    m_choice=malloc(size_m*sizeof(char));
    clrscr();
    switch(user->passenger_type){
        case 'n':
        case 's': //Both Normal and Student riders share the same menu.
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_NS);
                getline(&m_choice, &size_m, stdin);
                choice_m=atol(m_choice);
                if(choice_m<=0 || choice_m>4){//Error checking
                    using_choice_error(m_choice, &choice_m, size_m, 1, NULL);
                }
                switch(choice_m){
                    case 1: //For Bus passes
                            if(user->monthlyPasses[0]==false){ //If the user doesn't have the right pass active they are shown that and kicked back to the menu.
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else //Otherwise they are told to enjoy their day and is sent back to the menu.
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                            
                    case 2:
                            if(user->monthlyPasses[1]==false){
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                            
                    case 3:
                            if(user->monthlyPasses[2]==false){
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                    case 4:
                            return;
                    default: //Default case is to just assume that there is no active pass
                            printf(ERROR_WRONG_PASS);
                            getline(&m_choice, &size_m, stdin);
                            clrscr();
                            return;
                }
                break;
        case 'e':   //For elderly monthly riders.
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_E);
                getline(&m_choice, &size_m, stdin);
                choice_m=atol(m_choice);
                if(choice_m<=0 || choice_m>5){
                    using_choice_error(m_choice, &choice_m, size_m, 2, NULL);
                }
                switch(choice_m){
                    case 1: 
                            if(user->monthlyPasses[0]==false){
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                            
                    case 2:
                            if(user->monthlyPasses[1]==false){
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                            
                    case 3:
                            if(user->monthlyPasses[2]==false){
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                    case 4:
                            if(user->monthlyPasses[3]==false){
                                clrscr();
                                printf(ERROR_WRONG_PASS);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }else
                            {
                                clrscr();
                                printf(ENJOY_DAY);
                                getline(&m_choice, &size_m, stdin);
                                return;
                            }
                    case 5:
                            return;
                    default:
                            printf(ERROR_WRONG_PASS);
                            getline(&m_choice, &size_m, stdin);
                            clrscr();
                            return;
                }
                break;
        default:
                break;

    }
    free(m_choice);
}
//This is a submenu for pay-per-ride pass users. Pay-per-ride users have discounts according to whether its a weekend or not, if they are a student or an elderly rider. In addition subways and rail have an additional cost per stop, but right now its just a flat cost that may change in the future.
void pay_per_usage(struct Passenger *user){
    char* u_choice=NULL;//Get line purposes.
    char filename[]="prices.txt"; //Filenames for the various text files that hold the prices, discounts and price per stop
    char d_filename[]="discounts.txt";
    char s_filename[]="priceperstop.txt";
    int file_length=0; //get the file length in order to allocate memory to hold the above stated discounts etc.
    int d_file_length=0;
    int s_file_length=0;
    size_t u_size=1; //for getline
    long choice_u;
    bool is_weekend=false; //A boolean to check if it is the weekend. If it is the cost of the pass is 75% of the normal price. I'm assuming this is based off of the baseline price rather than the price as a whole.
    double* ride_prices;//Array to hold the ride prices
    double* stop_prices; //For stop prices
    double* discount_price; //for holding the discounts for students and elderly riders
    double finalPrice=0.0; //for holding the total
    file_length=get_num_lines(filename); //get the number of lines per file
    d_file_length=get_num_lines(d_filename);
    s_file_length=get_num_lines(s_filename);
    ride_prices=malloc(file_length*sizeof(double)); //allocate memory and create the arrays
    stop_prices=malloc(s_file_length*sizeof(double));
    discount_price=malloc(d_file_length*sizeof(double));
    get_ride_prices(ride_prices, filename); //get the various amount of prices and discounts
    get_price_per_stop(stop_prices, s_filename);
    get_discounts(discount_price, d_filename);
    u_choice=malloc(u_size*sizeof(char)); 
    if(local->tm_wday==0 || local->tm_wday==6){ //Check if today is a Saturday (6) or Sunday (0)
        is_weekend=true; //Set the flag
    }
    switch(user->passenger_type){//Menu depends on whether the user is a normal/student rider or an elderly rider.
        case 'n':
        case 's'://Normal and student riders share the same menu
                clrscr();
                printf(DATE, ctime(&current));
                printf(PAY_USAGE_NS, ride_prices[0], ride_prices[1], ride_prices[2]);
                getline(&u_choice, &u_size, stdin);
                choice_u=atoi(u_choice);
                if(choice_u<=0||choice_u>4){
                    using_choice_error(u_choice, &choice_u, u_size, 3, ride_prices);
                }
                switch(choice_u){
                    case 1: //For bus
                            clrscr();
                            if(is_weekend==true && user->passenger_type=='s'){//Various if-else statements to determine the final price via discounts and if the day is a weekend day.
                                finalPrice=(ride_prices[0]-(ride_prices[0]*discount_price[0]))*.75;
                            }else if(is_weekend==true && user->passenger_type=='n')
                            {
                                finalPrice=ride_prices[0]*.75;
                            }else if(user->passenger_type=='s'){
                                finalPrice=(ride_prices[0]-(ride_prices[0]*discount_price[0]));
                            }else
                            {
                                finalPrice=ride_prices[0];
                            }
                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            if(user->passenger_type=='s'){
                                printf("\nAs a Student, you are also eligible for a %f%% discount\n", discount_price[0]*100);
                            }
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;//Change the balance on the user pass.
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 2://For Subway
                            clrscr();
                            if(is_weekend==true && user->passenger_type=='s'){
                                finalPrice=((ride_prices[1]-(ride_prices[1]*discount_price[0]))*.75)+stop_prices[0];
                            }else if(is_weekend==true && user->passenger_type=='n')
                            {
                                finalPrice=(ride_prices[1]+stop_prices[0])*.75;
                            }else if(user->passenger_type=='s'){
                                finalPrice=(ride_prices[1]-(ride_prices[1]*discount_price[0]))+stop_prices[0];
                            }else
                            {
                                finalPrice=ride_prices[1]+stop_prices[0];
                            }
                            printf(DATE, ctime(&current));
                            printf(PRICE_STOP, stop_prices[0]); //Shows the user the price per stop. In the future you would have to determine where the user is at and how many stops away from the user their destination is in order to determine the overall price.
                            if(user->passenger_type=='s'){
                                printf("\nAs a Student, you are also eligible for a %f%% discount\n", discount_price[0]*100);
                            }
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 3: //For Rail
                            clrscr();
                            if(is_weekend==true && user->passenger_type=='s'){
                                finalPrice=((ride_prices[2]-(ride_prices[2]*discount_price[0]))*.75)+stop_prices[0];
                            }else if(is_weekend==true && user->passenger_type=='n')
                            {
                                finalPrice=(ride_prices[2]*.75)+stop_prices[0];
                            }else if(user->passenger_type=='s'){
                                finalPrice=(ride_prices[2]-(ride_prices[2]*discount_price[0]))+stop_prices[0];
                            }else
                            {
                                finalPrice=ride_prices[2]+stop_prices[0];
                            }
                            printf(DATE, ctime(&current));
                            printf(PRICE_STOP, stop_prices[0]);
                            if(user->passenger_type=='s'){
                                printf("\nAs a Student, you are also eligible for a %f%% discount\n", discount_price[0]*100);
                            }
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 4:
                    default:
                            return;
                }
                break;
        case 'e':
                clrscr();
                printf(DATE, ctime(&current));
                printf(PAY_USAGE_E, ride_prices[0], ride_prices[1], ride_prices[2], ride_prices[3]);
                getline(&u_choice, &u_size, stdin);
                choice_u=atoi(u_choice);
                if(choice_u<=0||choice_u>4){
                    using_choice_error(u_choice, &choice_u, u_size, 4, ride_prices);
                }
                switch(choice_u){
                    case 1:
                            clrscr();
                            if(is_weekend==true){
                                finalPrice=(ride_prices[0]-(ride_prices[0]*discount_price[1]))*75;
                            }
                            else
                            {
                                finalPrice=ride_prices[0]-(ride_prices[0]*discount_price[1]);
                            }

                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            printf("\nAs a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 2:
                            clrscr();
                            if(is_weekend==true){
                                finalPrice=((ride_prices[1]-(ride_prices[1]*discount_price[1]))*.75)+stop_prices[0];
                            }
                            else
                            {
                                finalPrice=(ride_prices[1]-(ride_prices[1]*discount_price[1]))+stop_prices[0];
                            }

                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            printf(PRICE_STOP, stop_prices[0]);
                            printf("\nAs a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 3:
                            clrscr();
                            if(is_weekend==true){
                                finalPrice=((ride_prices[2]-(ride_prices[2]*discount_price[1]))*.75)+stop_prices[0];
                            }
                            else
                            {
                                finalPrice=(ride_prices[2]-(ride_prices[2]*discount_price[1]))+stop_prices[0];
                            }

                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            printf(PRICE_STOP, stop_prices[0]);
                            printf("\nAs a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 4:
                            clrscr();
                            if(is_weekend==true){
                                finalPrice=(ride_prices[3]-(ride_prices[3]*discount_price[1]))*75;
                            }
                            else
                            {
                                finalPrice=ride_prices[3]-(ride_prices[3]*discount_price[1]);
                            }
                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            printf("\nAs a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getline(&u_choice, &u_size, stdin);
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getline(&u_choice, &u_size, stdin);
                            }
                            break;
                    case 5:
                    default:
                            return;
                }
                break;
        default:
                return;
    }
    free(ride_prices);//Freeing memory
    free(discount_price);
    free(stop_prices);
    free(u_choice);
}


