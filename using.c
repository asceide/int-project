#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "passenger.h"
#include "loadprices.h"

#define USING_MENU "Charlotte Transportation Authority\n\nWhat type of Pass are you using?\n1)Monthly\n2)Pay-per-ride\n3)Return\n"
#define MONTHLY_USAGE_E "What type of transportation will you be using?\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Special Bus\n"
#define PAY_USAGE_E "What type of transportation will you be using? PRICES:\nBus: %.2f\nSubway: %.2f (Baseline)\nCommuter Rail: %.2f (Baseline)\nSpecial Bus: %.2f\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Special Bus\n5)Go back to main menu\n"
#define MONTHLY_USAGE_NS "What type of transportation will you be using?\n1)Bus\n2)Subway\n3)Commuter Rail\n"
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
struct tm *local;
time_t current;

void using_choice_error(char* userChoice, long* choice, size_t size, int errorType, double* prices);
void monthly_usage(struct Passenger *user);
void pay_per_usage(struct Passenger *user);
void use_pass(struct Passenger *user);


void use_pass(struct Passenger *user){
    char* userChoice=NULL;
    size_t size_i=1;
    long choice;
    
    if(user->passenger_id<0){
        printf("You have not bought a pass yet. Please buy a pass first!\n");
        getchar();
        return;
    }
    time(&current);
    local=localtime(&current);
    userChoice=malloc(size_i*sizeof(char));
    system("clear");
    printf(USING_MENU);
    getline(&userChoice, &size_i, stdin);
    choice=atol(userChoice);
    if(choice<=0 || choice>3){
        using_choice_error(userChoice, &choice, size_i, 0, NULL);
    }
    if(user->isTWorker==true){
        printf("As a transportation worker, you're eligible for free transportation. Enjoy!\n");
        getchar();
        return;
    }
    switch(choice){
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
    free(userChoice);
}

void using_choice_error(char* userChoice, long* choice, size_t size, int errorType, double* prices){
    switch(errorType){
        case 0: 
                while(*choice<=0 || *choice>3){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(USING_MENU);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
                break;
        case 1:
                while(*choice<=0 || *choice>3){
                system("clear");
                printf(INCORRECT_CHOICE);
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_NS);
                getline(&userChoice, &size, stdin);
                *choice=atol(userChoice);
                }
                break;
        case 2:
                while(*choice<=0 || *choice>4){
                system("clear");
                printf(INCORRECT_CHOICE);
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_E);
                getline(&userChoice, &size, stdin);
                *choice=atol(userChoice);
                }
                break;
        case 3:
                while(*choice<=0 || *choice>4){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(DATE, ctime(&current));
                    printf(PAY_USAGE_NS, prices[0], prices[1], prices[2]);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
        case 4:
                while(*choice<=0 || *choice>5){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(DATE, ctime(&current));
                    printf(PAY_USAGE_E, prices[0], prices[1], prices[2], prices[3]);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
        default: 
                return;
    }
}

void monthly_usage(struct Passenger *user){
    char* m_choice=NULL;
    size_t size_m=1;
    long choice_m;

    m_choice=malloc(size_m*sizeof(char));
    system("clear");
    switch(user->passenger_type){
        case 'n':
        case 's':
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_NS);
                getline(&m_choice, &size_m, stdin);
                choice_m=atol(m_choice);
                if(choice_m<=0 || choice_m>3){
                    using_choice_error(m_choice, &choice_m, size_m, 1, NULL);
                }
                switch(choice_m){
                    case 1: 
                            if(user->monthlyPasses[0]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }
                            
                    case 2:
                            if(user->monthlyPasses[1]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }
                            
                    case 3:
                            if(user->monthlyPasses[2]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }
                    default:
                            printf(ERROR_WRONG_PASS);
                            getchar();
                            system("clear");
                            return;
                }
                break;
        case 'e':
                printf(DATE, ctime(&current));
                printf(MONTHLY_USAGE_E);
                getline(&m_choice, &size_m, stdin);
                choice_m=atol(m_choice);
                if(choice_m<=0 || choice_m>4){
                    using_choice_error(m_choice, &choice_m, size_m, 2, NULL);
                }
                switch(choice_m){
                    case 1: 
                            if(user->monthlyPasses[0]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }
                            
                    case 2:
                            if(user->monthlyPasses[1]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }
                            
                    case 3:
                            if(user->monthlyPasses[2]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }
                    case 4:
                            if(user->monthlyPasses[3]==false){
                                system("clear");
                                printf(ERROR_WRONG_PASS);
                                getchar();
                                system("clear");
                                return;
                            }else
                            {
                                printf(ENJOY_DAY);
                                getchar();
                                return;
                            }

                    default:
                            printf(ERROR_WRONG_PASS);
                            getchar();
                            system("clear");
                            return;
                }
                break;
        default:
                break;

    }
    free(m_choice);
}

void pay_per_usage(struct Passenger *user){
    char* u_choice=NULL;
    char filename[]="prices.txt";
    char d_filename[]="discounts.txt";
    char s_filename[]="priceperstop.txt";
    int file_length=0;
    int d_file_length=0;
    int s_file_length=0;
    size_t u_size=1;
    long choice_u;
    bool is_weekend=false;
    double* ride_prices;
    double* stop_prices;
    double* discount_price;
    double finalPrice=0.0;
    file_length=get_num_lines(filename);
    d_file_length=get_num_lines(d_filename);
    s_file_length=get_num_lines(s_filename);
    ride_prices=malloc(file_length*sizeof(double));
    stop_prices=malloc(s_file_length*sizeof(double));
    discount_price=malloc(d_file_length*sizeof(double));
    get_ride_prices(ride_prices, filename);
    get_price_per_stop(stop_prices, s_filename);
    get_discounts(discount_price, d_filename);
    u_choice=malloc(u_size*sizeof(char));
    if(local->tm_wday==0 || local->tm_wday==6){
        is_weekend=true;
    }
    switch(user->passenger_type){
        case 'n':
        case 's':
                system("clear");
                printf(DATE, ctime(&current));
                printf(PAY_USAGE_NS, ride_prices[0], ride_prices[1], ride_prices[2]);
                getline(&u_choice, &u_size, stdin);
                choice_u=atoi(u_choice);
                if(choice_u<=0||choice_u>4){
                    using_choice_error(u_choice, &choice_u, u_size, 3, ride_prices);
                }
                switch(choice_u){
                    case 1: 
                            system("clear");
                            if(is_weekend==true && user->passenger_type=='s'){
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
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                    case 2:
                            system("clear");
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
                            printf(PRICE_STOP, stop_prices[0]);
                            if(user->passenger_type=='s'){
                                printf("\nAs a Student, you are also eligible for a %f%% discount\n", discount_price[0]*100);
                            }
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                    case 3:
                            system("clear");
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
                                printf("As a Student, you are also eligible for a %f%% discount\n", discount_price[0]*100);
                            }
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                    default:
                            return;
                }
                break;
        case 'e':
                system("clear");
                printf(DATE, ctime(&current));
                printf(PAY_USAGE_E, ride_prices[0], ride_prices[1], ride_prices[2], ride_prices[3]);
                getline(&u_choice, &u_size, stdin);
                choice_u=atoi(u_choice);
                if(choice_u<=0||choice_u>4){
                    using_choice_error(u_choice, &choice_u, u_size, 4, ride_prices);
                }
                switch(choice_u){
                    case 1:
                            system("clear");
                            if(is_weekend==true){
                                finalPrice=(ride_prices[0]-(ride_prices[0]*discount_price[1]))*75;
                            }
                            else
                            {
                                finalPrice=ride_prices[0]-(ride_prices[0]*discount_price[1]);
                            }

                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            printf("As a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                    case 2:
                            system("clear");
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
                            printf("As a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                    case 3:
                            system("clear");
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
                            printf("As a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                    case 4:
                            system("clear");
                            if(is_weekend==true){
                                finalPrice=(ride_prices[3]-(ride_prices[3]*discount_price[1]))*75;
                            }
                            else
                            {
                                finalPrice=ride_prices[3]-(ride_prices[3]*discount_price[1]);
                            }
                            printf(DATE, ctime(&current));
                            printf(TRANSACTION, finalPrice, user->pass_balance);
                            printf("As a senior citizen a discount of %f%% has been applied.", discount_price[1]*100);
                            if(user->pass_balance<0 || ((user->pass_balance)-finalPrice)<0){
                                printf(TRAN_ERROR);
                                getchar();
                                return;
                            }else
                            {
                                user->pass_balance=user->pass_balance-finalPrice;
                                printf(AFTER_TRAN, user->pass_balance);
                                printf(ENJOY_DAY);
                                getchar();
                            }
                            break;
                }
                break;
        default:
                return;
    }
    free(ride_prices);
    free(discount_price);
    free(stop_prices);
    free(u_choice);
}


