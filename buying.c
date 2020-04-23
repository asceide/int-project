#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "passenger.h"
#include "loadprices.h"
#ifdef _WIN32
#include <Windows.h>
#include <bcrypt.h>
#pragma comment(lib, "Bcrypt")
#endif


#define BUYING_MENU_1 "What type of passenger are you\n1)Normal\n2)Student\n3)Elderly(Age 60+)\n4)Transportation Worker\n5)Go back to main menu\n"
#define BUYING_MENU_2NS "What type of transportation will you use?\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Go back to main menu\n"
#define BUYING_MENU_2E "What type of transportation will you use?\n1)Bus\n2)Subway\n3)Commuter Rail\n4)Special Bus\n5)Go back to main menu\n"
#define BUYING_MENU_3 "What type of pass do you want to buy?\n1)Monthly\n2)Pay-per-Ride\n3)Go back to main menu\n"
#define INCORRECT_CHOICE "You have selected an incorrect choice.\n"
#define RETURN_MENU "Returning to the main menu."
#define DATE "Todays date is: %s\n\n\n\n"
#define PAST_15_PRICE "As it is currently past the half of the month, the pass has a prorated cost of %.2f.\n Please insert payment now. \n(Press Enter to Continue)"
#define NORMAL_PRICE "The normal price for this ride will be %.2f.\n Please insert payment now. \n(Press Enter to Continue)"
#define DISCOUNT_AND_15_PRICE "As you are eligible for a %.0f%% discount, and since it is currently past the half of the month, the pass has a prorated cost of %.2f.\n Please insert payment now. \n(Press Enter to Continue)"
#define DISCOUNT_PRICE "As you are eligible for a %.0f%% discount, the total price of the pass has a cost of %.2f.\n Please insert payment now. \n(Press Enter to Continue)"

void choiceError(char* userChoice, long* choice, size_t size, int errorType); //This method will be use to check any errors in input and ensure that a correct input is given
bool securityCheck(); //This method is a security check to make sure that the person is actually a transportation worker using a password
void monthly(char rider_type, double* r_prices, double* d_prices, bool* pass); //This function is used to buy monthly tickets
void per_pass(char rider_type); //This function is used to buy pay-per-ride tickets
void buy_pass(struct Passenger *user);
int get_id();
double pay_per_ride();


void buy_pass(struct Passenger *user){
    char *userChoice=NULL;
    char typeOfPassenger;
    char passType;
    char dis_name[]="discounts.txt";
    char pri_name[]="monthlyprices.txt";
    size_t s_input=1;
    long choice=-1;
    long choice2=-1;
    double* prices;
    double* discounts;
    double totalPrice=0;
    int pass_id, dis_length, pri_length;
    dis_length=get_num_lines(dis_name);
    pri_length=get_num_lines(pri_name);
    discounts=malloc(dis_length*sizeof(double)); //allocating an array of size 2 to hold discounts. Index 0 would be students and Index 1 would be senior discounts
    prices=malloc(pri_length*sizeof(double)); //Allocating an array of size 4 to hol monthly prices. Index 0 are bus prices, Index 2 Subway, Index 3 Rail, Index 4 Elderly Special Bus
    userChoice=malloc(s_input*sizeof(char)); //Allocating for usage for getline();
    get_monthly_prices(prices, pri_name);//Gets the monthly prices for each transportation type.
    get_discounts(discounts, dis_name); //Gets the discount prices for Student and Elderly riders.
    system("clear");
    printf(BUYING_MENU_1);//Showing the main buying menu
    getline(&userChoice, &s_input, stdin);//Get the user input for the menu
    choice=atol(userChoice);//Converts the choice to an integer value
    if(choice<=0 || choice>5){//Checks if the user made an invalid input
        choiceError(userChoice, &choice, s_input, 0);
    }

    switch(choice){//Assigns the pass a passenger type according to their input
        case 1:
                typeOfPassenger='n'; //If they are a normal rider
                break;
        case 2:
                typeOfPassenger='s'; //If they are a student
                break;
        case 3: 
                typeOfPassenger='e'; //If they are an elderly rider
                break;
        case 4: if(securityCheck()==true){//If they selected that they're a transpo worker, they are sent for a security check via password.
                    typeOfPassenger='t';
                    totalPrice=0;
                    break;
                }else
                {
                    return;
                }
                
        case 5: 
        default: printf(RETURN_MENU); //Otherwise, return to the main menu if they decide to exit
                 return;
    }
    if(typeOfPassenger!=user->passenger_type && user->passenger_type!=0){
        for(int i=0; i<4; i++){
            user->monthlyPasses[i]=false;
        }
    }
    system("clear");
    printf(BUYING_MENU_3); //Get the type of pass the user would like to buy 
    getline(&userChoice, &s_input, stdin); //Again, get input and error check
    choice2=atol(userChoice);
    if(choice2<=0 || choice2>2){
        choiceError(userChoice, &choice2, s_input, 1);
    }else if(choice2==1){ //If the user wants to buy a monthly pass
        passType='m';
    }else if(choice2==2){ //If the user wants to buy a pay-per-ride pass
        passType='p';
    }else if(choice2==3){ //Otherwise, if the user wants to return to the main menu.
        printf(RETURN_MENU);
        return;
    }
   if(passType=='m'){ //If the user wants to buy a monthly pass
       monthly(typeOfPassenger, prices, discounts, user->monthlyPasses);
   }else if(passType=='p' && typeOfPassenger!='t'){
       totalPrice=pay_per_ride();
   }else if(passType=='p' && typeOfPassenger=='t'){
        printf("As a transportation worker you have a free pass!");

   }else
   
   {
       printf(RETURN_MENU);
       return;
   }
    pass_id=get_id();
    printf("\nYour passenger id is: %d , Please don't forget it!(Press enter to continue)", pass_id);
    getchar();
   /* if(user->passenger_type!=0 && user->passenger_type!=typeOfPassenger && passType=='m'){
        for(int i=0; i<4; i++){
            user->monthlyPasses[i]=false;
        }
    }*/
    user->passenger_id=pass_id;
    user->passenger_type=typeOfPassenger;
    user->pass_type=passType;
    user->pass_balance=totalPrice;

    free(userChoice);
    free(prices);
    free(discounts);
}
//Compliant RNG Generator. Although to be honest I am not to sure about the Windows version I have never used it before and the resources online were pretty scarce as to how exactly it works.
int get_id(){
#ifdef __unix
    struct timespec ts;
    if(timespec_get(&ts, TIME_UTC)==0){
        return 1;
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);
    int r=random()%10+1;
    return r;
#endif

#ifdef _WIN32
    BCRYPT_ALG_HANDLE Prov;
    int Buffer;
    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&Prov, BCRYPT_RNG_ALGORITHM, NULL, 0))){
        return 1;

    }
    if (!BCRYPT_SUCCESS(BCryptGenRandom(Prov, (PUCHAR) (&Buffer), sizeof(Buffer), 0))){
        return 1;
    }
    BCryptCloseAlgorithmProvider(Prov, 0);
    return Buffer%10+1;
#endif

}


//This Method is used to check for input errors. It gets the user choice, getline parameters and the type of error denoted by an int.
void choiceError(char* userChoice, long* choice, size_t size, int errorType){ 
    switch(errorType){
        case 0:
                while(*choice<=0 || *choice>5){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(BUYING_MENU_1);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
                break;
        case 1:
                while(*choice<=0 || *choice>3){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(BUYING_MENU_3);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
                break;
        case 2:
                while(*choice<=0 || *choice>4){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(BUYING_MENU_2NS);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
                break;
        case 3:
                while(*choice<=0 || *choice>5){
                    system("clear");
                    printf(INCORRECT_CHOICE);
                    printf(BUYING_MENU_2E);
                    getline(&userChoice, &size, stdin);
                    *choice=atol(userChoice);
                }
        default: break;
    }
}


bool securityCheck(){ //In normal circumstances, a 
    char* fileReader=NULL;
    char filename[]="password.txt";
    FILE *get_pass=fopen(filename,"r");
    size_t pw_buffer=100;
    char pw_type[100];
    char* userInput;
    size_t pass_size=10;
    if(get_pass==NULL){
        fprintf(stderr, "Cannot pen file: %s\n", filename);
        return false;
    }
    fileReader=malloc(pw_buffer*sizeof(char));
    getline(&fileReader, &pw_buffer, get_pass);
    sscanf(fileReader, "%s", pw_type);
    pw_type[strcspn(pw_type, "\r\n")]=0;
    system("clear");
    printf("Please input the special transportation worker password to gain access to this type of pass: ");
    userInput=(char*) malloc(pass_size);//For the password comparison, there is a need to allocate the memory prior to calling get line. Getline will realloc if more space is needed.
    getline(&userInput, &pass_size, stdin);
    userInput[strcspn(userInput, "\r\n")]=0; //This is used to strip away from the getLine string the newline and null. 
    
    if(strcmp(userInput, pw_type)==0){
        printf("The password was right! Here is your free pass!");
        return true;
    }
    else
    {
        printf("The Password was wrong, too bad!");
        return false;
    }
    free(userInput);
    free(fileReader);
}

void monthly(char rider_type, double* r_prices, double* d_prices, bool* pass){
    int day; //To hold the date
    char *userInput=NULL; //For user input
    size_t size=1;
    long choice=-1;
    double finalPrice;
    userInput=malloc(size*sizeof(char));
    struct tm *local; //For finding the exact date.
    time_t current;
    time(&current);
    local=localtime(&current);
    day = local->tm_mday;
    switch(rider_type){
        case 'n':
                system("clear");
                printf(DATE, ctime(&current));
                printf(BUYING_MENU_2NS);
                getline(&userInput, &size, stdin);
                choice=atol(userInput);
                if(choice<=0 || choice>4){
                    choiceError(userInput, &choice, size, 2);
                }
                switch(choice){
                    case 1:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=r_prices[0]*.5;
                                printf(PAST_15_PRICE, finalPrice);
                                getchar();
                                pass[0]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=r_prices[0];
                                printf(NORMAL_PRICE, finalPrice);
                                getchar();
                                pass[0]=true;
                                printf("Thank you");
                            }
                            break;
                    case 2:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=r_prices[1]*.5;
                                printf(PAST_15_PRICE, finalPrice);
                                getchar();
                                pass[1]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=r_prices[1];
                                printf(NORMAL_PRICE, finalPrice);
                                getchar();
                                pass[1]=true;
                                printf("Thank you");
                            }
                            break;
                    case 3:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=r_prices[2]*.5;
                                printf(PAST_15_PRICE, finalPrice);
                                getchar();
                                pass[2]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=r_prices[2];
                                printf(NORMAL_PRICE, finalPrice);
                                getchar();
                                pass[2]=true;
                                printf("Thank you");
                            }
                            break;     
                    default: break;                                   
                }
                break;
        case 's':
                system("clear");
                printf(BUYING_MENU_2NS);
                getline(&userInput, &size, stdin);
                choice=atol(userInput);
                if(choice<=0 || choice>4){
                    choiceError(userInput, &choice, size, 2);
                }
                switch(choice){
                    case 1:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[0]-(r_prices[0]*d_prices[0]))*.5;
                                double temp_dis=d_prices[0]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[0]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=r_prices[0]-(r_prices[0]*d_prices[0]);
                                double temp_dis=d_prices[0]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[0]=true;
                                printf("Thank you");
                            }
                            break;
                    case 2:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[1]-(r_prices[1]*d_prices[0]))*.5;
                                double temp_dis=d_prices[0]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[1]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=(r_prices[1]-(r_prices[1]*d_prices[0]));
                                double temp_dis=d_prices[0]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[1]=true;
                                printf("Thank you");
                            }
                            break;
                    case 3:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[2]-(r_prices[2]*d_prices[0]))*.5;
                                double temp_dis=d_prices[0]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[2]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=(r_prices[2]-(r_prices[2]*d_prices[0]));
                                double temp_dis=d_prices[0]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[2]=true;
                                printf("Thank you");
                            }
                            break;     
                    default: break;                                   
                }
                break;
        case 'e':
                system("clear");
                printf(BUYING_MENU_2E);
                getline(&userInput, &size, stdin);
                choice=atol(userInput);
                if(choice<=0 || choice>5){
                    choiceError(userInput, &choice, size, 3);
                }
                switch(choice){
                    case 1:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[0]-(r_prices[0]*d_prices[1]))*.5;
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[0]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=(r_prices[0]-(r_prices[0]*d_prices[1]));
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[0]=true;
                                printf("Thank you");
                            }
                            break;
                    case 2:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[1]-(r_prices[1]*d_prices[1]))*.5;
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[1]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=(r_prices[1]-(r_prices[1]*d_prices[1]));
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[1]=true;
                                printf("Thank you");
                            }
                            break;
                    case 3:
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[2]-(r_prices[2]*d_prices[1]))*.5;
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[2]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=(r_prices[2]-(r_prices[2]*d_prices[1]));
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[2]=true;
                                printf("Thank you");
                            }
                            break;    
                    case 4: //I assume that the elderly don't get a 50% addition discount to their special bus since it is only reserved for them in the first place
                            system("clear");
                            printf(DATE, ctime(&current));
                            if(day>15){
                                finalPrice=(r_prices[3]-(r_prices[3]*d_prices[1]))*.5;
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_AND_15_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[3]=true;
                                printf("Thank you");
                            }else
                            {
                                finalPrice=(r_prices[3]-(r_prices[3]*d_prices[1]));
                                double temp_dis=d_prices[1]*100;
                                printf(DISCOUNT_PRICE, temp_dis, finalPrice);
                                getchar();
                                pass[3]=true;
                                printf("Thank you");
                            }
                            break;  
                    default: break;                                   
                }
                break;
        case 't':
                printf("As a transportation worker you have a free pass!");
                for(int i=0; i<4; i++){
                    pass[i]=true;
                }
                break;
        default: return;
    }
    free(userInput);
}

double pay_per_ride(){
    char* money;
    double cash;
    size_t buffer=100;
    money=malloc(buffer*sizeof(char));
    
    system("clear");
    printf("Please insert the amount of money you want to use for this pass (Minimum of $1.00) (Use 0 to exit): ");
    getline(&money, &buffer, stdin);
    if(isdigit(money[0])==0 || strtod(money, NULL)<0){
        while(isdigit(money[0])==0 || strtod(money, NULL)<0){
            system("clear");
            printf("You have made an invalid input.\nPlease insert the amount of money you want to use for this pass (Use 0 to exit)\n");
            getline(&money, &buffer, stdin);
        }
    }
    cash=strtod(money, NULL);
    free(money);
    return cash;

}